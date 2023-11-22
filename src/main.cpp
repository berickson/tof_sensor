#include "async_SparkFun_TMF882X_Library.h"
//#include "async_qwiic_tmf882x.h"

Async_SparkFun_TMF882X tmf882x;

#define SAMPLE_TIMEOUT_MS  3000

void measurement_callback(struct tmf882x_msg_meas_results *results) {
    Serial.printf("%d - ", results->result_num);
    for(uint32_t i = 0; i < results->num_results; ++i) {
      Serial.printf("%5dmm", results->results[i].distance_mm);
    }
    Serial.println();
}

void setup() {

    delay(3000);
    Serial.begin(921600);
    Serial.println("");




    for(uint8_t address = 1; address < 128; ++address) {
      if(!tmf882x.begin(address)) {
        continue;
      }
      Serial.printf("Connected to TMF882X at address %d ", address);
      break;
    }   
    char application_version[80];
    tmf882x.getApplicationVersion(application_version, 79);
    Serial.printf("Application version: %s\n", application_version);
 
    Serial.printf("i2c_address: %u\n",tmf882x.getI2CAddress());
    tmf882x.setI2CAddress_fix(60);
    while(true) {
      Serial.println(11);
      delay(1000);
    }
    // Serial.println("set the i2address to 32");
    // Serial.printf("i2c_address: %u\n",tmf882x.getI2CAddress());

    while(true) {
      delay(1);
    };

    tmf882x.setMeasurementHandler(measurement_callback);

    const int ms_delay_between_samples = 1;
    tmf882x.setSampleDelay(ms_delay_between_samples);
}

void loop() {
    bool one_per_loop = true;
    // currently, one_per_loop is slower, probably because it keeps reinitializing the sensor
    // also, for some reason it is sending some duplicate results
    if (one_per_loop) {
      tmf882x.async_startMeasuring();
      while(true) {
        tmf882x.async_updateMeasuring();
        Serial.println("processing between measurements");
        delay(1);
      }
    } else {
        tmf882x.startMeasuring(1000, SAMPLE_TIMEOUT_MS);
    } 
}