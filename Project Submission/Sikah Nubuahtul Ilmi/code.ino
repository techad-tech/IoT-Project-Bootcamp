#define TRIGGER_PIN 2
#define ECHO_PIN 3

const float MAX_HEIGHT = 60; // Ketinggian maksimal kotak sampah dalam sentimeter

void setup() {
  Serial.begin(115200);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout
  float distance = (duration / 2.0) * 0.0343; // Convert to centimeters

  if (duration == 0) {
    Serial.println("Measurement timeout.");
  } else {
    Serial.print("Distance: ");
    Serial.println(distance);

    if (distance < 20 && distance > 3) {
      Serial.println("Garbage bin is almost full.");
    }

    if (distance > MAX_HEIGHT) {
      Serial.println("Garbage bin height exceeded.");
    }

    if (distance >= 2 && distance < 3) {
      Serial.println("Please empty the garbage.");
    }
  }

    delay(1000);
}