#include <WiFi.h>
#include <DHT.h>
#include "PubSubClient.h"


const int ldrPin = 32;  // ADC pin connected to the LDR

#define DHTPIN 14
#define DHTTYPE DHT22

DHT dhtkita(DHTPIN,DHTTYPE);

char clientId[50];

WiFiClient wifiClient;                // Membuat objek wifiClient
PubSubClient mqttClient(wifiClient);  // Membuat  objek mqttClient dengan konstruktor objek WiFiClient (Permintaan dari Lib)

char *mqttServer = "broker.hivemq.com";
// String mqttServer = "broker.hivemq.com";
int mqttPort = 1883;


void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Callback - ");
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  reconnect();
  pinMode(ldrPin, INPUT);
  dhtkita.begin();
  mqttClient.setServer(mqttServer, mqttPort); // Mengatur detail broker target  yang digunakan
  mqttClient.setCallback(callback);           // jika kita ingin menerima pesan untuk langganan yang dibuat oleh klien
  connectToBroker();
  // reconnect();
}


void loop() {
  // menghitung nilai humidity
  float hum = dhtkita.readHumidity();
  Serial.print("Nilai Hum : ");
  Serial.print(hum);

  char humString[8];
  dtostrf(hum,1,1,humString); 
  mqttClient.publish("techad/santri/hum", humString);

  // menghitung nilai temperature
  float temp= dhtkita.readTemperature();
  Serial.print("\t Nilai Temp: ");
  Serial.println(temp);

  char tempString[8];
  dtostrf(temp,1,1,tempString); 
  mqttClient.publish("techad/santri/temp", tempString);

  // menghitung nilai lux
  int ldr = analogRead(ldrPin);
  Serial.print("\t LDR Value: ");
  Serial.println(ldr);

  char ldrString[8];
  dtostrf(ldr,1,1,ldrString); 
  mqttClient.publish("techad/santri/ldr", ldrString);

  delay(1000);
}
