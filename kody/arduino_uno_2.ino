#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial mySerial(2, 3); 
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

Servo myServo;

// ---- LDR + LED ----
#define LDR_PIN A0
#define LED_PIN 6      

// ---- Fireplace LED ----
#define FIRE_LED 10    

//  ---- Prah tmy  ---- 
int darkThreshold = 100;

// ---- Premenné pre krb efekt ----
unsigned long previousMillis = 0;
int flickerInterval = 60;

void setup() {

  // Servo
  myServo.attach(9);
  myServo.write(0);   

  // LED výstupy
  pinMode(LED_PIN, OUTPUT);
  pinMode(FIRE_LED, OUTPUT);

  // Náhodné blikanie
  randomSeed(analogRead(A1));

  // Fingerprint
  finger.begin(57600);
  if (!finger.verifyPassword()) {
    while (1); // zastaví program ak senzor nefunguje
  }
}

void loop() {

  // ---- LDR LED  ----
  int ldrValue = analogRead(LDR_PIN);

  if (ldrValue < darkThreshold) {
    digitalWrite(LED_PIN, HIGH);   // svieti
  } else {
    digitalWrite(LED_PIN, LOW);    // vypnutá
  }

  // ---- Fireplace efekt ----
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= flickerInterval) {
    previousMillis = currentMillis;

    int randomBrightness = random(150, 255);
    analogWrite(FIRE_LED, randomBrightness);

    flickerInterval = random(40, 120);
  }

  // ---- Fingerprint systém ----
  int id = getFingerprintID();

  if (id >= 0) {
    myServo.write(90);   // odomkni
    delay(5000);
    myServo.write(0);    // zamkni
  }

  delay(100);
}

int getFingerprintID() {
  if (finger.getImage() != FINGERPRINT_OK) return -1;
  if (finger.image2Tz() != FINGERPRINT_OK) return -1;
  if (finger.fingerSearch() != FINGERPRINT_OK) return -1;
  return finger.fingerID;
}
