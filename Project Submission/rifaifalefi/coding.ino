#include "DHT.h"            //Library DHT
#define DHTPIN 19           //Inisialisasi DHT pada pin D5
#define DHTTYPE DHT11       //Inisialisasi tipe DHT
 
DHT dht (DHTPIN,DHTTYPE);   //Mengenalkan pin dan tipe DHT
float t;  //Variabel penampung nilai suhu
int LED = 13;   //LED merah pada pin D2
int LEDK = 3;   //LED kuning pada pin D3
int LEDH = 4;   //LED hijau pada pin D4
 
void setup() {
  Serial.begin(9600);     //Mulai komunikasi serial
  dht.begin();            //Mulai DHT
  pinMode(LED, OUTPUT);  //Setting LED sebagai output
  
}

void loop() {
  float t = dht.readTemperature();    //Baca suhu
  Serial.print("Suhu : ");
  Serial.print(t);              //Tampilkan suhu
  Serial.println(" *C");
  if (t <= 1){                 
    digitalWrite(LED, HIGH);
    Serial.println("ON");
  }
  else if (t >= 1){            
    digitalWrite(LED, LOW);
    Serial.println("OFF");
  }
  delay(1000);                  //Jeda pembacaan 1000ms
}
