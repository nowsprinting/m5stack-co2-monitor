//
// Created by Koji Hasegawa on 2022/10/15.
//

#ifndef M5STACK_TEMPERATURE_MONITOR_SRC_HELPER_M5PERIPHERALS_H_
#define M5STACK_TEMPERATURE_MONITOR_SRC_HELPER_M5PERIPHERALS_H_

#ifdef M5STACK_BASIC
#define ESP32_CORE_NUM 2
#define M5_LCD
#define M5_TF_CARD
#define M5_BUTTON_A 39
#define M5_BUTTON_B 38
#define M5_BUTTON_C 37
#define M5_SPEAKER 25
#define M5_SCL 22
#define M5_SDA 21
#endif

#ifdef M5STACK_FIRE
#define ESP32_CORE_NUM 2
#define M5_LCD
#define M5_TF_CARD
#define M5_BUTTON_A 39
#define M5_BUTTON_B 38
#define M5_BUTTON_C 37
#define M5_SPEAKER 25
#define M5_IMU
#define M5_PSRAM
#define M5_SCL 22
#define M5_SDA 21
#endif

#ifdef M5GO_FIRE_BOTTOM
#define M5_MIC 34
#define M5_LED_BAR 15
#define M5_LED_BAR_NUM 10
#endif

#ifdef ATOM_LITE
#define ESP32_CORE_NUM 2
#define M5_LED_BAR 27
#define M5_LED_BAR_NUM 1
#define M5_BUTTON_A 39
#define M5_IR 12
#define M5_SCL 21
#define M5_SDA 25
#endif

#endif //M5STACK_TEMPERATURE_MONITOR_SRC_HELPER_M5PERIPHERALS_H_
