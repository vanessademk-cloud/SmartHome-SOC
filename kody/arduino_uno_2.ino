#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Servo.h>

// -------------------- Fingerprint --------------------
SoftwareSerial mySerial(2, 3);   // RX, TX pre fingerprint
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// -------------------- Servo --------------------
Servo myServo;
#define SERVO_PIN 9

// -------------------- Soil moisture + pump --------------------
#define SOIL_PIN A2
#define PUMP_PIN 12
int soilThreshold = 600;   // uprav podľa reálnych hodnôt senzora

// -------------------- LDR + LED --------------------
#define LDR_PIN A0
#define LED_PIN 6
int darkThreshold = 500;

// -------------------- Servo stav --------------------
bool doorOpen = false;
unsigned long servoOpenedTime = 0;
unsigned long servoOpenDuration = 3000;

// -------------------- Fingerprint kontrola --------------------
unsigned long previousFingerMillis = 0;
unsigned long fingerInterval = 500;

void setup() {
  Serial.begin(9600);

  // Servo
  myServo.attach(SERVO_PIN);
  myServo.write(0);

  // Piny
  pinMode(LED_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);

  // Relé vypnuté na začiatku (active LOW)
  digitalWrite(PUMP_PIN, HIGH);

  // Fingerprint
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Fingerprint senzor je pripraveny");
  } else {
    Serial.println("Fingerprint senzor nebol najdeny");
  }
}

void loop() {
  handleLDR();
  handleSoilAndPump();
  handleFingerprint();
  handleServoClose();
}

// --------------------------------------------------
// Fotorezistor a LED
// --------------------------------------------------
void handleLDR() {
  int ldrValue = analogRead(LDR_PIN);

  if (ldrValue < darkThreshold) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}

// --------------------------------------------------
// Vlhkost pôdy a čerpadlo
// --------------------------------------------------
void handleSoilAndPump() {
  int soilValue = analogRead(SOIL_PIN);

  Serial.print("Soil value: ");
  Serial.println(soilValue);

  if (soilValue > soilThreshold) {
    digitalWrite(PUMP_PIN, LOW);   // zapne relé = zapne čerpadlo
  } else {
    digitalWrite(PUMP_PIN, HIGH);  // vypne relé = vypne čerpadlo
  }
}

// --------------------------------------------------
// Fingerprint a servo
// --------------------------------------------------
void handleFingerprint() {
  if (millis() - previousFingerMillis >= fingerInterval) {
    previousFingerMillis = millis();

    uint8_t p = finger.getImage();
    if (p != FINGERPRINT_OK) return;

    p = finger.image2Tz();
    if (p != FINGERPRINT_OK) return;

    p = finger.fingerFastSearch();
    if (p == FINGERPRINT_OK) {
      Serial.println("Odtlacok rozpoznany");

      myServo.write(90);
      doorOpen = true;
      servoOpenedTime = millis();
    }
  }
}

// --------------------------------------------------
// Automatické zatvorenie serva
// --------------------------------------------------
void handleServoClose() {
  if (doorOpen && millis() - servoOpenedTime >= servoOpenDuration) {
    myServo.write(0);
    doorOpen = false;
    Serial.println("Servo zatvorene");
  }
}
