#include <WiFi.h> // Untuk memakai library
#include "PubSubClient.h"


int pinPir = 2; // Mendefinisikan pin & variabel
int activity = 0;


char clientId[50];
WiFiClient wifiClient;  // Membuat objek wifiClient
// Membuat  objek mqttClient dengan konstruktor objek WiFiClient (Permintaan dari Lib)
PubSubClient mqttClient(wifiClient);  
char *mqttServer = "broker.hivemq.com"; // Alamat broker 
int mqttPort = 1883;


void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Callback - ");
  Serial.print("Message: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
  }
}

void reconnect(){
  WiFi.mode(WIFI_STA);  // Assign esp as Station
  WiFi.begin("Wokwi-GUEST", "");
  while(WiFi.status()!=WL_CONNECTED){
    Serial.println(".");
    delay(100);
  }
  Serial.print("Connection Status: ");
  Serial.println(WiFi.status());
  Serial.println("ESP32 is Connected");
}

void connectToBroker() {
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


void setup() {  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("PIR Room Monitoring System");
  reconnect();
  pinMode(pinPir, INPUT); // Mode pin
  // Mengatur detail broker target  yang digunakan
  mqttClient.setServer(mqttServer, mqttPort); 
  // Jika kita ingin menerima pesan untuk langganan yang dibuat oleh klien
  mqttClient.setCallback(callback);           
  connectToBroker();  
}


void loop() { // put your main code here, to run repeatedly:
  int readPir = digitalRead(pinPir); //baca nilai yang diberikan oleh sensor pir 
  if(readPir == HIGH)  //nilai high jika sensor pir mendeteksi aktivitas manusia
  { 
    activity = 1; // Activity 1, maka ada manusia
    Serial.println()("MOTION DETECTED, SOMEONE'S INSIDE THE ROOM!!!");
  }
  else //nilai low jika sensor pir tidak mendeteksi aktivitas manusia
  {
    activity = 0; // Activity 0, maka tidak ada manusia
    Serial.println("NO MOTION DETECTED, NO ONE'S INSIDE THE ROOM...");
  } 
  String activityString;
  activityString = String(activity);  // Converts integer to string
  // Publish activity data to broker
  mqttClient.publish("techad/hanif/activity", activityString); 
  delay(1000); // delay 1 detik
}