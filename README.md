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

1. CLionで開くとき、"Open Project Wizard"ウィンドウが表示される。"Debug"は消して、"+"をクリックして"m5stack-fire"をつくる
1. Tools | PlatformIO | Re-Init（CMakeListsPrivate.txtがつくられる）
1. Run/Debug Configurationが無いので、"+"をクリックして"PlatformIO Upload"を追加。Before launchの"Build"は削除
1. macOSでCH9102F（M5Stack Basic/FIRE v2.6以降のシリアルIC）を使用するとき、platformio.iniにupload_portを追加

M5Stackのシリアルポートを確認

```(bash)
ls /dev/cu.wch*
```

platformio.iniに追加

```(ini)
upload_port = /dev/cu.wchusbserial54790356591
```
