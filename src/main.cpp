#include <Arduino.h>
#include <M5Stack.h>
#include <SparkFun_SCD30_Arduino_Library.h>
#include <Ambient.h>

SCD30 scd30;

void setup() {
    M5.begin(true, false, true, true);

    if (scd30.begin() == false) {
        M5.Lcd.println("Air sensor not detected. Please check wiring. Freezing...");
        while (1);
    }
    //The SCD30 has data ready every two seconds
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
    M5.Lcd.printf("Tmp %5.1f c\n", temp);
    M5.Lcd.printf("Hum %5.1f %%\n", hum);
}

unsigned long lastMillis = NAN;

void loop() {
    measureSCD30();
    displayLcd();

    delay(2000);
}
