//
// Created by Koji Hasegawa on 2022/10/15.
//

#ifndef M5STACK_CO2_MONITOR_SRC_CO2_SENSOR_H_
#define M5STACK_CO2_MONITOR_SRC_CO2_SENSOR_H_

#include <arduino.h>

struct co2_sensor_values {
  uint16_t co2 = 0;
  float temp = NAN;
  float hum = NAN;
};

bool SetupCo2Sensor();

void GetCO2(co2_sensor_values &);

#endif //M5STACK_CO2_MONITOR_SRC_CO2_SENSOR_H_
