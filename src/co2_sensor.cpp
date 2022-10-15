//
// Created by Koji Hasegawa on 2022/10/15.
//

#include "co2_sensor.h"

#include <SparkFun_SCD30_Arduino_Library.h>

SCD30 scd30;

bool SetupCo2Sensor() {
  return scd30.begin();
}

void GetCO2(co2_sensor_values &values) {
  if (scd30.dataAvailable()) {
    values.co2 = scd30.getCO2();
    values.temp = scd30.getTemperature();
    values.hum = scd30.getHumidity();
  }
}
