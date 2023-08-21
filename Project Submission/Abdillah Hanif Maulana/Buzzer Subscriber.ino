#include <WiFi.h> // Untuk memakai library
#include "PubSubClient.h"

int pinBuzzer = 32; // Mendefinisikan pin output

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqttServer = "broker.hivemq.com";
int mqttPort = 1883;
char clientId[50];
WiFiClient espClient;
PubSubClient client(espClient);


void setupMQTT() {
  // Mengatur detail broker target  yang digunakan
  client.setServer(mqttServer, mqttPort); 
  // Jika kita ingin menerima pesan untuk langganan yang dibuat oleh klien
  client.setCallback(callback);           
}


void mqttReconnect() {
 while (!client.connected()) {
   Serial.print("Attempting MQTT connection...");
   long r = random(1000);
   sprintf(clientId, "clientId-%ld", r);
   if (client.connect(clientId)) {
     Serial.print(clientId);
     Serial.println("Connected");
     client.subscribe("techad/hanif/activity");
   } else {
     Serial.print("Failed, rc=");
     Serial.print(client.state());
     Serial.println("Try again in 5 seconds...");
     delay(5000);
   }
 }
}


void callback(char* topic, byte* message, unsigned int length) {
 Serial.print("Message: ");
 String stMessage;
  for (int i = 0; i < length; i++) {
   Serial.print((char)message[i]);
   stMessage += (char)message[i];
 }
  // Mengkonversi nilai dari string ke float
  float fltMessage = stMessage.toFloat(); 
  // Jika nilai aktivitas == 1, buzzer akan berbunyi
  if (strcmp(topic, "techad/hanif/activity") == 1) { 
    Serial.print("\t Activity: ");
    Serial.println(fltMessage);
    digitalWrite(pinBuzzer,HIGH);
    Serial.println()("MOTION DETECTED, SOMEONE'S INSIDE THE ROOM!!!");
  }
  else // Jika nilai aktivitas == 0, buzzer tidak akan berbunyi
  {
    Serial.print("\t Activity: ");
    Serial.println(fltMessage);
    digitalWrite(pinBuzzer,LOW);
    Serial.println("NO MOTION DETECTED, NO ONE'S INSIDE THE ROOM...");
  } 
 Serial.println();
}


void wifiConnect() {
 WiFi.mode(WIFI_STA);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
 }
}


void setup() {
 Serial.begin(115200);
 pinMode(pinBuzzer, OUTPUT); // Mendefinisikan mode pin
 delay(10);
 Serial.println();
 Serial.print("Connecting to ");
 Serial.println(ssid);
 wifiConnect();
 Serial.println("");
 Serial.println("WiFi connected");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
 setupMQTT(); // setup koneksi ke broker
}


void loop() {
 delay(10);
 if (!client.connected()) {
   mqttReconnect();
 }
 client.loop();
}