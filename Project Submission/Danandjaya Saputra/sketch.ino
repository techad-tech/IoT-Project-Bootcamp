#include "WiFi.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define PIN_TRIG 21
#define PIN_ECHO 19
#define PIN_LED_R 14
#define PIN_LED_G 27
#define PIN_LED_B 26
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
char *mqttServer = "broker.hivemq.com";
int mqttPort = 1883;

int valSensor = 0;
StaticJsonDocument<256> jsonDoc;

void coonectWifi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.print("Connection Status: ");
  Serial.println(WiFi.status());
  Serial.println("ESP32 is connected to internet");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Callback - ");
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
  }
}

void connectToBrokerMqtt() {
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) {
    Serial.println("Reconnecting to MQTT Broker..");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("Connected.");
    }      
  }
}

void setLedIndicator(int mode){
  if(mode == 1){ //RED
    digitalWrite(PIN_LED_R, 0);
    digitalWrite(PIN_LED_G, 1);
    digitalWrite(PIN_LED_B, 1);
  }
  if(mode == 2){ //GREEN
    digitalWrite(PIN_LED_R, 1);
    digitalWrite(PIN_LED_G, 0);
    digitalWrite(PIN_LED_B, 1);
  }
  if(mode == 3){ //BLUE
    digitalWrite(PIN_LED_R, 1);
    digitalWrite(PIN_LED_G, 1);
    digitalWrite(PIN_LED_B, 0);
  }
  delay(1000);
}

int readSensorUltrasonik(){
  // Start a new measurement:
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Read the result:
  int duration = pulseIn(PIN_ECHO, HIGH);

  return (duration / 58);
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  coonectWifi(); 
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(callback);
  connectToBrokerMqtt();
}

void loop() {
  valSensor = readSensorUltrasonik();
  if(valSensor > 250){
    setLedIndicator(3);
  }else if(valSensor > 100){
    setLedIndicator(2);
  }else{
    setLedIndicator(1);
  }
  jsonDoc["sensor"] = String(valSensor);
  jsonDoc["latitude"] = "0.9209418589442037";
  jsonDoc["longitude"] = "104.4542413758129";
  char messageSender[128];
  serializeJson(jsonDoc, messageSender);
  mqttClient.publish("danandjaya/smarttrashbin", messageSender);
  Serial.println(messageSender); 
}