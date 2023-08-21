#include <WiFi.h>
#include "PubSubClient.h"


const int ldrPin = 32;  // ADC pin connected to the LDR

const char* ssid = "Wokwi-GUEST";
const char* password = "",
const char* mqttServer = "broker.hivemq.com";
int mqttPort = 1883;

char clientId[50];

WiFiClient espClient;               
PubSubClient mqttClient(espClient); 


void setup() {
  Serial.begin(115200);
  mqttClient.setServer(mqttServer, mqttPort); 
  mqttClient.setCallback(callback);          
}


void mqttReconnect(){
  while (!mqttClient.connected()){
    Serial.print("Attempting MQTT connection...");
    long r = random(1000);
    sprintf(clientId, "clientId-%1d", r);
    if (mqttClient.connect(clientId)){
      Serial.print(clientId);
      Serial.println("connected");
      mqttClient.subscribe("techad/santri/temp");
      mqttClient.subscribe("techad/santri/hum");
      mqttClient.subscribe("techad/santri/ldr");
    }
    else {
      Serial.print("failed, rc= ");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Callback - ");
  Serial.print("Message:");
  String stMessage;
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    stMessage += ((char)message[i]);
  }
  // mengonversi nilai string menjadi float
  float fltMessage1 = stMessage.toFloat();
  float fltMessage2 = stMessage.toFloat();
  float fltMessage3 = stMessage.toFloat();

  if (strcmp(topic, "techad/santri/temp") == 0){
    Serial.print("\t Temperature: ");
    Serial.println(fltMessage1);
    if (fltMessage1 < 18){
      digitalWrite(22, HIGH);
      digitalWrite(21, LOW);
      digitalWrite(4, LOW);
    }
    else if (fltMessage1 > 30){
      digitalWrite(22, HIGH);
      digitalWrite(21, HIGH);
      digitalWrite(4, HIGH);
    }
    else {
      digitalWrite(22, HIGH);
      digitalWrite(21, LOW);
      digitalWrite(4, HIGH);
    }
  }
  else if (strcmp(topic, "techad/santri/hum") == 0){
    Serial.print("\t Humidity: ");
    Serial.println(fltMessage2);
    if (fltMessage2 < 65){
      digitalWrite(22, HIGH);
      digitalWrite(21, HIGH);
      digitalWrite(4, HIGH);
    }
    else if (fltMessage2 > 95){
      digitalWrite(22, HIGH);
      digitalWrite(21, LOW);
      digitalWrite(4, LOW);
    }
    else {
      digitalWrite(22, HIGH);
      digitalWrite(21, LOW);
      digitalWrite(4, HIGH);
    }
  }
  else if (strcmp(topic, "techad/santri/ldr") == 0){
    Serial.print("\t Lux: ");
    Serial.println(fltMessage3);
    if (fltMessage3 > 2045){
      digitalWrite(34, HIGH);
      digitalWrite(35, HIGH);
      digitalWrite(26, HIGH);
    }
    else if (fltMessage3 < 1291){
      digitalWrite(34, HIGH);
      digitalWrite(35, LOW);
      digitalWrite(26, LOW);
    }
    else {
      digitalWrite(34, HIGH);
      digitalWrite(35, LOW);
      digitalWrite(26, HIGH);
    }
  }
  Serial.println();
}

void wificonnect(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
}
