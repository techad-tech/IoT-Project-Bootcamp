// Project IoT Monitoring Tambak Udang Vanname // 
// MONITORING KOLAM UDANG VANAME // Sampling data on the north side of the pond.

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHTesp.h"
#include "ThingsBoard.h"
#include <ESP32Servo.h>

// Initialize ThingsBoard client
WiFiClient espClient;
// Initialize ThingsBoard instance
ThingsBoard tb(espClient);
// the Wifi radio's status
int status = WL_IDLE_STATUS;

#define TOKEN               "pSc39SFe6z8bwzPXDVfy" // Change the token based on your account TB
#define THINGSBOARD_SERVER  "thingsboard.cloud" // FIXED

const int heatPin = 12;
const int bzrPin = 4;
const int servoPin = 5;
const int relayPin = 2;

//SENSOR DHT22
const int DHT_PIN = 15;
DHTesp dhtSensor;
Servo servo;
int pos = 0;
LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 16, 2);

//LCD 16X2
void spinner() {
  static int8_t counter = 0;
  const char* glyphs = "\xa1\xa5\xdb";
  LCD.setCursor(15, 1);
  LCD.print(glyphs[counter++]);
  if (counter == strlen(glyphs)) {
    counter = 0;
  }
}

void stdbyScreen() {
  LCD.setCursor(8, 0);
  LCD.println("SMART");
  LCD.setCursor(0, 1);
  LCD.println("POUND-V1");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  pinMode(heatPin, OUTPUT);
  pinMode(bzrPin, OUTPUT);
  servo.attach(servoPin, 500, 2400);
  pinMode(relayPin, OUTPUT);

  LCD.init();
  LCD.backlight();
  LCD.setCursor(0, 0);
  LCD.print("Connecting to ");
  LCD.setCursor(0, 1);
  LCD.print("WiFi ");
  InitWiFi();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000); // this speeds up the simulation
   if (WiFi.status() != WL_CONNECTED) {
    reconnect();
  }

  if (!tb.connected()) {
    // Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);

    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("Failed to connect");
      return;
    }
  }
  Serial.println("Sending data...");
  // Uploads new telemetry to ThingsBoard using MQTT.
  // See https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api
  // for more details

  TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  tb.sendTelemetryInt("temperature", data.temperature);
  tb.sendTelemetryFloat("humidity", data.humidity);
  Serial.print("Temperature : ");
  Serial.print(data.temperature);
  Serial.print(" Humidity : ");
  Serial.println(data.humidity);
  Serial.println("Data on the north side of the pond . . . .");
  checkTemperature();
  lcdMonitor();
  tb.loop();
}

void checkTemperature(){
TempAndHumidity  data = dhtSensor.getTempAndHumidity();
if(data.temperature < 26){
  digitalWrite(heatPin, HIGH);
  digitalWrite(bzrPin, HIGH);
  LCD.setCursor(8, 0);
  LCD.println("WARNING");
  LCD.setCursor(0, 1);
  LCD.println("Cold Temperature");
 // LCD.Clear();
}else if(data.temperature > 34){
for (pos = 0; pos <= 180; pos += 1) {
    servo.write(pos);
    delay(15);
  }
  for (pos = 180; pos >= 0; pos -= 1) {
    servo.write(pos);
    delay(15);
  }
LCD.println("WARNING");
LCD.setCursor(0, 1);
LCD.println("Hot Temperature");
//LCD.Clear();
}else{
  digitalWrite(relayPin, HIGH);
  lcdMonitor();
}
}

void lcdMonitor(){
  TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  LCD.setCursor(8, 0);
  LCD.println("SMRTPND-V1");
  LCD.setCursor(0, 1);
  LCD.println("Temperature : ");
  LCD.print(data.temperature);
}
