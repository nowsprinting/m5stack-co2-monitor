//
// Created by Koji Hasegawa on 2022/10/01.
//

#include <Arduino.h>
#include <M5Stack.h>
#include <Ambient.h>

#include "helper/m5peripherals.h"
#include "helper/slack.h"
#include "co2_sensor.h"

const int kSensorInterval = 5000;
const int kAmbientInterval = 60000;
const int kCo2AlertThreshold = 1500;
const int kCo2AlertStep = 100;

WiFiClient client;
Ambient ambient;

void setup() {
    M5.begin(true, false, true, true);
    M5.Lcd.setBrightness(10);   // デフォルトは80
    M5.Lcd.setTextSize(2);

#ifdef ENABLE_CO2
    M5.Lcd.println("Initializing CO2 sensor");
    if (SetupCo2Sensor() == false) {
        M5.Lcd.println("Air sensor not detected. Please check wiring. Freezing...");
        while (1);
    }
    //The SCD30 has data ready every two seconds
#endif

    M5.Lcd.print("Initializing Wi-Fi");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  //  Wi-Fi APに接続
    while (WiFi.status() != WL_CONNECTED) {  //  Wi-Fi AP接続待ち
        delay(500);
        M5.Lcd.print(".");
    }
    M5.Lcd.println();
    M5.Lcd.printf("Wi-Fi connected. IP address: ");
    M5.Lcd.println(WiFi.localIP());

    M5.Lcd.print("Initializing Ambient");
    ambient.begin(AMBIENT_CHANNEL_ID, AMBIENT_WRITE_KEY, &client); // チャネルIDとライトキーを指定してAmbientの初期化

    M5.Speaker.begin();
    M5.Speaker.setVolume(1);  // デフォルトは8
}

// CO2センサ測定値
co2_sensor_values values;

void displayBatteryLevel() {
    int8_t level = M5.Power.getBatteryLevel();
    if (level < 0) {
        return;
    }
    if (level <= 25) {
        M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
    } else if (level <= 75) {
        M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
    } else {
        M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    }
    M5.Lcd.printf("Battery %d%%\n", level);
}

bool lastAmbientSendResult = true;

void displayError() {
    if (!lastAmbientSendResult) {
        M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
        M5.Lcd.printf("Ambient send error!\n");
        NotifyToSlack("Ambientへのデータ送信に失敗しました", true);
    }
}

bool beep = true;

void displayLcd() {
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.setTextSize(4);
    M5.Lcd.printf("CO2 %5d ppm\n", values.co2);
    M5.Lcd.println();
    M5.Lcd.printf("Tmp %5.1f 'c\n", values.temp);
    M5.Lcd.printf("Hum %5.1f %%\n", values.hum);

    // 以下、ステータス系
    M5.Lcd.println();
    M5.Lcd.setTextSize(2);
    displayBatteryLevel();
    displayError();

    if (beep && values.co2 >= kCo2AlertThreshold) {
        M5.Speaker.beep();
        delay(100);
        M5.Speaker.mute();
    }
}

uint16_t nextAlertCo2 = kCo2AlertThreshold;

void slackNotification(uint16_t co2) {
    if (co2 >= nextAlertCo2) {
        NotifyToSlack("部屋の二酸化炭素濃度が" + String(nextAlertCo2) + "ppmを超えました", true);
        nextAlertCo2 += kCo2AlertStep;
        Serial.printf("Update CO2 threshold to %d", nextAlertCo2);
    } else if ((co2 < (nextAlertCo2 - kCo2AlertStep * 1.2)) && (nextAlertCo2 > kCo2AlertThreshold)) {
        nextAlertCo2 -= kCo2AlertStep;
        Serial.printf("Update CO2 threshold to %d", nextAlertCo2);
    }
}

unsigned long nextMeasureTime = 0; // 次回センサ計測ms
unsigned long nextAmbientTime = 0; // 次回Ambient送信ms

void loop() {
    M5.update();
    unsigned long now = millis();   // TODO: ラップアラウンドするのであれば対策が必要

    if (M5.BtnC.wasPressed()) {
        beep = !beep;
        if (beep) {
            M5.Speaker.beep();
            delay(100);
            M5.Speaker.mute();
        }
    }

    if (now > nextMeasureTime) {
#ifdef ENABLE_CO2
        GetCO2(values);
        slackNotification(values.co2);
#endif
        displayLcd();
        nextMeasureTime = now + kSensorInterval;
    }

    if (now > nextAmbientTime) {
        if (values.co2 > 0) {
            ambient.set(1, values.co2);
            ambient.set(2, values.temp);
            ambient.set(3, values.hum);
        }
        lastAmbientSendResult = ambient.send();
        nextAmbientTime = now + kAmbientInterval;
    }
}
