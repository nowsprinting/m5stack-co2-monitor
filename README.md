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

1. CLionで開くとき、"Open Project Wizard"ウィンドウが表示される。"Debug"は消して、"+"をクリックして"m5stack-core-esp32"をつくる
1. Run Configurationが無いので、"+"をクリックして"PlatformIO Upload"を追加。Before launchの"Build"は削除していい
1. Tools | PlatformIO | Re-Initを実行


### CH9102F（M5Stack BASIC/FIRE v2.6のシリアルIC）macOSドライバv1.7を使用するとき

ビルドしたソフトウェアをアップロードするシリアルポートを指定する必要がある。
シリアルポート指定は、platformio.iniファイルか、PLATFORMIO_UPLOAD_PORT環境変数で行なう

参考：[PlatformIO for CLionで #M5Stack へのuploadポートを環境変数で指定する - やらなイカ？](https://www.nowsprinting.com/entry/2022/10/09/020549)


### ビルドフラグを上書きするとき

ビルドフラグは、 platformio.iniにある設定をPLATFORMIO_BUILD_FLAGS環境変数で上書きできる。
ただし、PlatformIO for CLion v222.3739.54時点では指定できない。CLion起動前に設定するか、build & uploadをCLIで実行する

#### Makefileの例

```bash
.PHONY: upload-fire
upload-fire:
	PLATFORMIO_BUILD_FLAGS='-D M5STACK_FIRE -D M5GO_FIRE_BOTTOM -D WIFI_SSID=\"SSID\" -D WIFI_PASSWORD=\"PWD\"' \
	PLATFORMIO_UPLOAD_PORT=$(shell ls /dev/cu.wch*) \
	pio run --target upload --environment m5stack-fire

.PHONY: upload-atom
upload-atom:
	PLATFORMIO_BUILD_FLAGS='-D M5STACK_ATOM_LITE -D WIFI_SSID=\"SSID\" -D WIFI_PASSWORD=\"PWD\"' \
	pio run --target upload --environment m5stack-atom
```
