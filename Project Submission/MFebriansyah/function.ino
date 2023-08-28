void reconnect(){
  WiFi.mode(WIFI_STA);
  WiFi.begin("Wokwi-GUEST", "");
  while(WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }
  Serial.println("");
  Serial.print("STATUS KONEKSI = ");
  Serial.println(WiFi.status());
  Serial.println("ESP32 Telah Terkoneksi");
}
void connect_to_broker(String clientName) {
  Serial.println("Connecting to MQTT Broker...");
  while (!Client.connected()) {
      Serial.println("Reconnecting to MQTT Broker..");
      String clientId = "ESP32Client-" + clientName;
      // clientId += String(random(0xffff), HEX);
      Serial.println(clientId);
      if (Client.connect(clientId.c_str())) {
        Serial.print(clientId);
        Serial.println();
        Serial.println("Device to Broker Connected.");
      }
      else {
        Serial.print("failed, rc=");
        Serial.print(Client.state());
        Serial.println();
        Serial.println("try again in 1 seconds");
        delay(1000);
      }       
  }
}
void mqtt_publish(String yourTopic, float yourPayload){
  char yourPayloadString[8];
  dtostrf(yourPayload, 1, 2, yourPayloadString);
  Client.publish(yourTopic.c_str(), yourPayloadString);
}
