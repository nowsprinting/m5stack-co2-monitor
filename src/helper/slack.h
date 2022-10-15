//
// Created by Koji Hasegawa on 2022/10/10.
//

#ifndef M5STACK_CO2_MONITOR_SRC_HELPER_SLACK_H_
#define M5STACK_CO2_MONITOR_SRC_HELPER_SLACK_H_

#include <Arduino.h>

void notifyToSlack(String message, bool here);

#endif //M5STACK_CO2_MONITOR_SRC_HELPER_SLACK_H_
