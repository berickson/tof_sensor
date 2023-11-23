#include "async_SparkFun_TMF882X_Library.h"
#include "tof_bin_image.h"
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

void set_address() {

    Serial.println("updating firmware");
    tmf882x.loadFirmware(tof_bin_image, tof_bin_image_length);
    tmf882x.setI2CAddress_fix(0x40);
    Serial.println("done");
    while(true) {
      delay(1000);
    }

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
 
    Serial.printf("i2c_address: 0x%02x\n",tmf882x.getI2CAddress());
    // set_address();

    tmf882x.setMeasurementHandler(measurement_callback);

    const int ms_delay_between_samples = 1;
    tmf882x.setSampleDelay(ms_delay_between_samples);
    Serial.println("starting");
}

void loop() {
    bool one_per_loop = true;
    // currently, one_per_loop is slower, probably because it keeps reinitializing the sensor
    // also, for some reason it is sending some duplicate results
    if (one_per_loop) {
      tmf882x.async_startMeasuring();
      while(true) {
        tmf882x.async_updateMeasuring();
        // Serial.println("processing between measurements");
        delay(1);
      }
    } else {
        tmf882x.startMeasuring(1000, SAMPLE_TIMEOUT_MS);
    } 
}