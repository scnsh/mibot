#include <M5Stack.h>
#include <NeoPixelBus.h>
#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <M5Bala.h>

// for wifi
WiFiClient wifiClient;
const char* SSID = "ssid";
const char* PASSWORD = "パスワード";

M5Bala m5bala(Wire);

// ==================== MQTT =====================
const char *THING_NAME = "M5Stack";
PubSubClient mqttClient;
const char* BROKER_IP = "PCのIP";
// command
int command = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  String buf_t = String(topic);
  M5.Lcd.println(buf_t);

  payload[length] = '\0';
  String buf_s = String((char*) payload);
  M5.Lcd.println(buf_s);

  if (buf_s == "w")
  {
    command = 1;
  }else if (buf_s == "s"){
    command = 2;
  }else if (buf_s == "a"){
    command = 3;
  }else if (buf_s == "d"){
    command = 4;
  }else{
    command = 0;
  }
  M5.Lcd.printf("cmd:%d\n", command);
}

void connectToBroker() {
  M5.Lcd.println("trying to connect MQTT broker");
  mqttClient.setServer(BROKER_IP, 1883);
  mqttClient.setCallback(callback);
  mqttClient.setClient(wifiClient);
  if (!mqttClient.connect(THING_NAME)) {
    M5.Lcd.println(mqttClient.state());
  }
  mqttClient.subscribe("m5stack/control/#");
}

void setup() {
	// Power ON Stabilizing...
	delay(500);
	M5.begin();

	// Init I2C
	Wire.begin();
	Wire.setClock(400000UL);  // Set I2C frequency to 400kHz
	delay(500);

  // wifi connetction
  WiFi.begin(SSID, PASSWORD);
  M5.Lcd.println("Wait for WiFi... ");

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Lcd.println(".");
  }
  M5.Lcd.println("WiFi connected");
  M5.Lcd.println("IP address: ");
  M5.Lcd.println(WiFi.localIP());

  connectToBroker();
  M5.Lcd.println("success to connect MQTT broker");

	// Init M5Bala
	m5bala.begin();
}

void loop() {

  mqttClient.loop();

  if (command == 1) {
    m5bala.setMotor(128, 128);
  } else if (command == 2) {
    m5bala.setMotor(-128, -128);
  } else if (command == 3) {
    m5bala.setMotor(-128, 128);
  } else if (command == 4) {
    m5bala.setMotor(128, -128);
  }else{
    m5bala.setMotor(0, 0);
  }
  Serial.printf("cmd: %d\n", command);

  M5.update();
}