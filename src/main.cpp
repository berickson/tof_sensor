#include "SparkFun_TMF882X_Library.h"

SparkFun_TMF882X tmf882x;

#define SAMPLE_TIMEOUT_MS  3000

void measurement_callback(struct tmf882x_msg_meas_results *results) {
    Serial.printf("%d - ", results->result_num);
    for(uint32_t i = 0; i < results->num_results; ++i) {
      Serial.printf("%5dmm", results->results[i].distance_mm);
    }
    Serial.println();
}

void setup() {

    delay(500);
    Serial.begin(921600);
    Serial.println("");

    while(!tmf882x.begin()) {
        Serial.println("Error - The TMF882X failed to initialize - is the board connected?");
        delay(1000);
    }

    tmf882x.setMeasurementHandler(measurement_callback);

    const int ms_delay_between_samples = 1;
    tmf882x.setSampleDelay(ms_delay_between_samples);
}

void loop() {
    bool one_per_loop = true;
    // currently, one_per_loop is slower, probably because it keeps reinitializing the sensor
    // also, for some reason it is sending some duplicate results
    if (one_per_loop) {
      tmf882x_msg_meas_results results;
      const int success = 1, timeout_ms = 100;
      if(tmf882x.startMeasuring(results, timeout_ms) ==success) { 
        measurement_callback(&results);
      }
      delay(30);
    } else {
        tmf882x.startMeasuring(1000, SAMPLE_TIMEOUT_MS);
    } 
}