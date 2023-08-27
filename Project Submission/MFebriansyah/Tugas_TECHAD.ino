#include <WiFi.h>
#include <DHT.h>
#include "PubSubClient.h"

const int DHTPIN = 19;
const int PIR_PIN = 22;
const int Button_PIN = 2;

const char* mqttServer = "broker.hivemq.com";
int mqttPort = 1883;

#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

//char clientID[50];
String myClientID = "001";
String Topic_Suhu = "TECHAD_Tugas/MFebriansyah/temp";
String Topic_Lembab = "TECHAD_Tugas/MFebriansyah/hum";
String Topic_Switch = "TECHAD_Tugas/MFebriansyah/sw";
String Topic_PIR = "TECHAD_Tugas/MFebriansyah/pir";

WiFiClient espClient;
PubSubClient Client(espClient);

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Callback - ");
  Serial.print("Message:");
  String stMessage;
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    stMessage += (char)message[i];
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(Button_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  
  reconnect();
  dht.begin();
  Client.setServer(mqttServer, mqttPort);
  Client.setCallback(callback);
  connect_to_broker(myClientID);
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  
  Serial.print("Temp: " + String(temperature, 2) + "°C || ");
  Serial.print("Humidity: " + String(humidity, 1) + "% ");

  mqtt_publish(Topic_Suhu,temperature);
  mqtt_publish(Topic_Lembab,humidity);

  if(digitalRead(Button_PIN) == HIGH){
                                      Serial.print("|| SW: ON ");
                                      Client.publish(Topic_Switch.c_str(), "ON");
                                     }
  else {
        Serial.print("|| SW: OFF ");
        Client.publish(Topic_Switch.c_str(), "OFF");
       }
  
  if(digitalRead(PIR_PIN) == HIGH){
                                   Serial.println("|| PIR: AKTIF");
                                   Client.publish(Topic_PIR.c_str(), "AKTIF");
                                   }
  else {
        Serial.println("|| PIR: TIDAK AKTIF");
        Client.publish(Topic_PIR.c_str(), "TIDAK AKTIF");
       }
  delay(2000);
}
