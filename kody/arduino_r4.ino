#define BLYNK_TEMPLATE_ID "################"
#define BLYNK_TEMPLATE_NAME "###################"
#define BLYNK_AUTH_TOKEN "##########################"

#include <Wire.h>
#include <WiFiS3.h>
#include <BlynkSimpleWifi.h>
#include <DHT.h>

// ---- WiFi ----
char ssid[] = "######";
char pass[] = "#######";

// ---- DHT11 ----
#define DHTPIN 11
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ---- Gas Sensor ----
#define GAS_PIN A0
#define GAS_THRESHOLD 600

// ---- Flame Sensor ----
#define FLAME_PIN 8

// ---- BUZZER ----
#define BUZZER_PIN 2

// ---- FAN MOTOR ----
#define FAN_PIN 3
int fanSpeed = 0;

// ---- LED + TLAČÍTKO ----
#define BUTTON4_PIN 4
#define LED4_PIN 12

bool led4State = false;
int lastReading = HIGH;
int buttonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

// ---- Auto Fan ----
const int HIGH_TEMP = 25;
const int LOW_TEMP  = 24;
bool autoFan = false;

// ---- Blynk slider pre FAN ----
BLYNK_WRITE(V7) {
  fanSpeed = param.asInt();
  analogWrite(FAN_PIN, fanSpeed);
}

void setup() {
  Serial.begin(9600);

  pinMode(FLAME_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);

  pinMode(BUTTON4_PIN, INPUT_PULLUP);
  pinMode(LED4_PIN, OUTPUT);
  digitalWrite(LED4_PIN, LOW);

  analogWrite(FAN_PIN, 0);

  dht.begin();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "64.225.16.22", 8080);

  Serial.println("Start programu");
}

void handleButtonLED() {
  int reading = digitalRead(BUTTON4_PIN);

  if (reading != lastReading) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        led4State = !led4State;
        digitalWrite(LED4_PIN, led4State);

        Serial.print("LED4: ");
        Serial.println(led4State ? "ZAPNUTA" : "VYPNUTA");
      }
    }
  }

  lastReading = reading;
}

void loop() {
  Blynk.run();
  handleButtonLED();

  static unsigned long timer = 0;
  if (millis() - timer > 5000) {
    timer = millis();

    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (!isnan(t)) {
      if (t >= HIGH_TEMP && !autoFan) {
        fanSpeed = 255;
        analogWrite(FAN_PIN, fanSpeed);
        Blynk.virtualWrite(V7, fanSpeed);
        autoFan = true;
      }

      if (t <= LOW_TEMP && autoFan) {
        fanSpeed = 0;
        analogWrite(FAN_PIN, fanSpeed);
        Blynk.virtualWrite(V7, fanSpeed);
        autoFan = false;
      }
    }

    int gasValue = analogRead(GAS_PIN);
    float gasPercent = gasValue * 100.0 / 1023.0;
    int flameDetected = !digitalRead(FLAME_PIN);

    if (flameDetected || gasValue > GAS_THRESHOLD) {
      tone(BUZZER_PIN, 1000);
    } else {
      noTone(BUZZER_PIN);
    }

    if (!isnan(t)) Blynk.virtualWrite(V0, t);
    if (!isnan(h)) Blynk.virtualWrite(V1, h);
    Blynk.virtualWrite(V3, gasPercent);
    Blynk.virtualWrite(V4, flameDetected);
    Blynk.virtualWrite(V7, fanSpeed);
    Blynk.virtualWrite(V6, led4State);

    Serial.print("T: "); Serial.print(t);
    Serial.print("  H: "); Serial.print(h);
    Serial.print("  Gas: "); Serial.print(gasValue);
    Serial.print("  Flame: "); Serial.print(flameDetected);
    Serial.print("  LED4: "); Serial.println(led4State);
  }
}
