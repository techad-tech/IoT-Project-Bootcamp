void reconnect(){
  WiFi.mode(WIFI_STA);  // Assign esp as Station
  WiFi.begin("Wokwi-GUEST", "");
  while(WiFi.status()!=WL_CONNECTED){
    Serial.println(".");
    delay(1000);
  }
  Serial.print("Status Koneksi : ");
  Serial.println(WiFi.status());
  Serial.println("ESP Telah Terkoneksi");
}

void connectToBroker(){
  Serial.println("Kami mencoba connect ke broker");
  while(!mqttClient.connected()){
    String clientID = "ESP00011";
    if(mqttClient.connect(clientID.c_str())){
      Serial.println("ESP kita sudah terkoneksi");
    }
  }
}

// void connectToBroker() {
//   Serial.println("Connecting to MQTT Broker...");
//   while (!mqttClient.connected()) {
//       Serial.println("Reconnecting to MQTT Broker..");
//       String clientId = "ESP32Client-";
//       clientId += String(random(0xffff), HEX);
      
//       if (mqttClient.connect(clientId.c_str())) {
//         Serial.println("Connected.");
//       }      
//   }
// }
