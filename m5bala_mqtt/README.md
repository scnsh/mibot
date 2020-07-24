# m5balas with mqtt control

# setup

## package install
```
$ sudo apt install mosquitto
$ sudo apt install mosquitto-clients
```

## wifi setup
wifi接続できるようにする

``` main.cpp
const char* SSID = "ssid";
const char* PASSWORD = "パスワード";
```

mqttのbrokerを設定する(開発PCを想定)
``` main.cpp
const char* BROKER_IP = "PCのIP";
```

## build & send
m5stack を PCと接続した状態でvscodeの `→` アイコンを押せば build & send される

## 動作確認

下記コマンドを実行して、前進するか確認
```
$ mosquitto_pub -h localhost -t m5stack/control -m w
```

停止
```
$ mosquitto_pub -h localhost -t m5stack/control -m f
```
