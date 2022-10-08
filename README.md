# m5stack-co2-monitor



## Hardware

### [SCD30](https://www.sparkfun.com/products/15112)

NDIR方式のCO2センサ、温湿度センサ

[M5Stack用CO2モニターキット](https://www.switch-science.com/products/6923)として購入

#### SCD30 - M5Stack結線

- VDD - 3.3V
- GND - GND
- TX/SCL
- RX/SDA - G21
- RDY
- PWM
- SEL

I2Cアドレス：0x61



## How to development

### cloneしたとき

1. CLionで開くとき、"Open Project Wizard"ウィンドウが表示される。"Debug"は消して、"+"をクリックして"m5stack-core-esp32"をつくる（nameはplatformio.iniのenv名）
1. Run/Debug Configurationが無いので、"+"をクリックして"PlatformIO Upload"を追加。Before launchの"Build"は削除していい
1. Tools | PlatformIO | Re-Init


### CH9102F（M5Stack Basic v2.6のシリアルIC）macOSドライバv1.7を使用するとき

ビルドしたソフトウェアをアップロードするシリアルポートを指定する必要がある

M5Stackを接続しているシリアルポートを確認

```bash
ls /dev/cu.wch*
```

platformio.iniに追加

```ini
upload_port = /dev/cu.wchusbserialXXXXXXXXXXX
```

もしくは、.bashrcに追加

```ini
export PLATFORMIO_UPLOAD_PORT=/dev/cu.wchusbserialXXXXXXXXXXX
```

もしくは、uploadをCLIで実行

```bash
PLATFORMIO_UPLOAD_PORT=$(ls /dev/cu.wch*) pio run --target upload -e m5stack-core-esp32
```

もしくは

```bash
pio run --target upload -e m5stack-core-esp32 --upload-port $(ls /dev/cu.wch*)
```
