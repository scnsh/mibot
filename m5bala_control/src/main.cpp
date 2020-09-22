#include <M5Stack.h>
#include <MPU6050_tockn.h>
#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>

// for wifi
WiFiClient wifiClient;
const char* SSID = "teamx";
const char* PASSWORD = "Gbo2TU55SmbPZ7h7";


MPU6050 mpu6050(Wire);

// ==================== MQTT =====================
const char *THING_NAME = "M5StackController";
PubSubClient mqttClient;
const char* BROKER_IP = "192.168.100.93";
// command
int command = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  String buf_t = String(topic);
  // M5.Lcd.println(buf_t);

  payload[length] = '\0';
  String buf_s = String((char*) payload);

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
  // M5.Lcd.println(buf_s);
  // M5.Lcd.printf("cmd:%d\n", command);
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
	M5.begin();

  Wire.begin();

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

  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

void loop() {

  mqttClient.loop();
  mpu6050.update();

  if (mpu6050.getAngleX() > 50)
  {
    mqttClient.publish("m5stack/control", "w");
  }
  else if (mpu6050.getAngleX() < 0)
  {
    mqttClient.publish("m5stack/control", "");
  }

  M5.Lcd.startWrite();
  M5.Lcd.setCursor(0, 150);
  M5.Lcd.printf("angleX : %f\n", mpu6050.getAngleX());
  M5.Lcd.printf("angleY : %f\n", mpu6050.getAngleY());
  M5.Lcd.printf("angleZ : %f\n", mpu6050.getAngleZ());
  M5.Lcd.endWrite();

  M5.update();
}