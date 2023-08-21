int buttonPin = 2;  
int led1 = 13;
int led2 = 12;
int buzzer = 5;


int count = 0; 


void setup() {
  pinMode(buttonPin, INPUT); 
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);        
}


void loop() {
  // jika tombol ditekan, menambahkan nomor
  if (digitalRead(buttonPin) == HIGH) {
    count++;
    
     if (count == 3) {
      count = 1;
    }
  
    if (count == 1) {
      tone(buzzer, 1000);
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      delay(1000);
      Serial.println("Selamat datang");
      noTone(buzzer);
    } else {
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      Serial.println("Selamat jalan");
    }
    delay(250);
  }


}