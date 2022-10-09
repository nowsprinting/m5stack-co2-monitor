#include <Arduino.h>
#include <M5Stack.h>
#include <SparkFun_SCD30_Arduino_Library.h>
#include <SparkFunBME280.h>
#include <Ambient.h>

SCD30 scd30;
BME280 bme280;

WiFiClient client;
Ambient ambient;

void setup() {
    M5.begin(true, false, true, true);

    M5.Lcd.setTextSize(2);
    M5.Lcd.println("Initializing SCD30 sensor");
    if (scd30.begin() == false) {
        M5.Lcd.println("Air sensor not detected. Please check wiring. Freezing...");
        while (1);
    }
    //The SCD30 has data ready every two seconds

    M5.Lcd.println("Initializing BME280 sensor");
    bme280.setI2CAddress(0x76);
    if (bme280.beginI2C() == false) //Begin communication over I2C
    {
        Serial.println("The sensor did not respond. Please check wiring.");
        while (1); //Freeze
    }

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

struct scd30_measured_values {
    uint16_t co2 = NAN;
    float temp = NAN;
    float hum = NAN;
};

scd30_measured_values scd30values;
uint16_t lastCo2 = NAN;

void measureSCD30(scd30_measured_values &values) {
    if (scd30.dataAvailable()) {
        values.co2 = scd30.getCO2();
        values.temp = scd30.getTemperature();
        values.hum = scd30.getHumidity();
    }
}

struct bme280_measured_values {
    float pressure = NAN;
    float temp = NAN;
    float hum = NAN;
};

bme280_measured_values bme280values;

void measureBME280(bme280_measured_values &values) {
    values.pressure = bme280.readFloatPressure();
    values.temp = bme280.readTempC();
    values.hum = bme280.readFloatHumidity();
}

bool lastAmbientSendResult = true;

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

void displayError() {
    if (!lastAmbientSendResult) {
        M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
        M5.Lcd.printf("Ambient send error!\n");
    }
}

void displayLcd(scd30_measured_values &scd30values, bme280_measured_values &bme280values) {
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.setTextSize(4);

    // SCD30
    M5.Lcd.printf("CO2 %5d ppm\n", scd30values.co2);
    M5.Lcd.println();
    M5.Lcd.printf("Tmp %5.1f 'c\n", scd30values.temp);
    M5.Lcd.printf("Hum %5.1f %%\n", scd30values.hum);

    // BME280
    M5.Lcd.println();
    M5.Lcd.setTextSize(2);
    M5.Lcd.printf("Prs %5d hPa\n", bme280values.pressure);
    M5.Lcd.printf("Tmp %5.1f 'c  Hum %5.1f %%\n", bme280values.temp, bme280values.hum);

    // 以下、ステータス系
    M5.Lcd.println();
    M5.Lcd.setTextSize(2);
    displayBatteryLevel();
    displayError();
}

unsigned long nextMeasureTime = 0; // 次回センサ計測ms
unsigned long nextAmbientTime = 0; // 次回Ambient送信ms

void loop() {
    M5.update();
    unsigned long now = millis();   // TODO: ラップアラウンドするのであれば対策が必要

    if (now > nextMeasureTime) {
        measureSCD30(scd30values);
        measureBME280(bme280values);
        displayLcd(scd30values, bme280values);
        nextMeasureTime = now + 5000;
    }

    if (now > nextAmbientTime) {
        ambient.set(1, scd30values.co2);
        ambient.set(2, scd30values.temp);
        ambient.set(3, scd30values.hum);
        lastAmbientSendResult = ambient.send();
        nextAmbientTime = now + 60000;
    }
}
