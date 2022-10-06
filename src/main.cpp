#include <Arduino.h>
#include <M5Stack.h>
#include <SparkFun_SCD30_Arduino_Library.h>
#include <Ambient.h>

SCD30 scd30;

WiFiClient client;
Ambient ambient;

void setup() {
    M5.begin(true, false, true, true);

    M5.Lcd.println("Initializing SCD30 sensor");
    if (scd30.begin() == false) {
        M5.Lcd.println("Air sensor not detected. Please check wiring. Freezing...");
        while (1);
    }
    //The SCD30 has data ready every two seconds

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
}

uint16_t co2 = NAN;
uint16_t lastCo2 = NAN;
float temp = NAN;
float hum = NAN;

void measureSCD30() {
    if (scd30.dataAvailable()) {
        co2 = scd30.getCO2();
        temp = scd30.getTemperature();
        hum = scd30.getHumidity();
    }
}

void displayLcd() {
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.setTextSize(4);
    M5.Lcd.printf("CO2 %5d ppm\n", co2);
    M5.Lcd.println();
    M5.Lcd.printf("Tmp %5.1f 'c\n", temp);
    M5.Lcd.printf("Hum %5.1f %%\n", hum);
}

unsigned long nextMeasureTime = 0; // 次回センサ計測ms
unsigned long nextAmbientTime = 0; // 次回Ambient送信ms

void loop() {
    unsigned long now = millis();   // TODO: ラップアラウンドするのであれば対策が必要

    if (now > nextMeasureTime) {
        measureSCD30();
        displayLcd();
        Serial.println("measure");
        nextMeasureTime = now + 5000;
    }

    if (now > nextAmbientTime) {
        ambient.set(1, co2);
        ambient.set(2, temp);
        ambient.set(3, hum);
        ambient.send();
        Serial.println("send");
        nextAmbientTime = now + 60000;
    }
}
