#include <WiFi.h>
#include <PubSubClient.h>

int ledPin = 4;                // Pilih pin untuk LED
int inputPin = 18;               // Pilih pin input (untuk sensor PIR)
int pirState = LOW;             // Kita asumsikan awalnya tidak ada gerakan terdeteksi
int val = 0;                    // Variabel untuk membaca status pin

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

char *mqttServer = "broker.hivemq.com";
int mqttPort = 1883;

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Callback - ");
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
  }
}

void reconnect() {
  WiFi.mode(WIFI_STA);
  WiFi.begin("Wokwi-GUEST", "");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.print("Status Koneksi: ");
  Serial.println(WiFi.status());
  Serial.println("ESP Telah Terkoneksi");
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

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");

  pinMode(ledPin, OUTPUT);      // Deklarasikan LED sebagai output
  pinMode(inputPin, INPUT);     // Deklarasikan sensor sebagai input
  reconnect();
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(callback);
  connectToBroker();
}

void loop() {
  val = digitalRead(inputPin);
  char statusMessage[50];
  
  if (val == HIGH) {
    digitalWrite(ledPin, HIGH);
    if (pirState == LOW) {
      Serial.println("Motion detected!");
      
      // Mengisi buffer dengan pesan dan nilai sensor
      snprintf(statusMessage, sizeof(statusMessage), "Motion detected! PIR value: %d", val);
      
      // Kirim status gerakan dengan nilai sensor sebagai pesan MQTT
      mqttClient.publish("zadry/antimaling/status", statusMessage);
      
      pirState = HIGH; // Set pirState menjadi HIGH untuk menunjukkan gerakan sedang terdeteksi
    }
  } else {
    digitalWrite(ledPin, LOW);
    if (pirState == HIGH) {
      Serial.println("Motion ended!");
      
      // Mengisi buffer dengan pesan dan nilai sensor
      snprintf(statusMessage, sizeof(statusMessage), "Motion ended! PIR value: %d", val);
      
      // Kirim status gerakan dengan nilai sensor sebagai pesan MQTT
      mqttClient.publish("zadry/antimaling/status", statusMessage);
      
      pirState = LOW; // Set pirState menjadi LOW untuk menunjukkan gerakan berakhir
    }
  }

  delay(1000); // Tunda 1 detik
}



