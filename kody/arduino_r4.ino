#define BLYNK_TEMPLATE_ID "##################"
#define BLYNK_TEMPLATE_NAME "#####################"
#define BLYNK_AUTH_TOKEN "###########################"

#include <Wire.h>
#include <WiFiS3.h>
#include <BlynkSimpleWifi.h>
#include <DHT.h>

// ---- WiFi ----
char ssid[] = "###########";
char pass[] = "###########";

// ---- DHT11 ----
#define DHTPIN 11
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ---- Gas Sensor ----
#define GAS_PIN A0
#define GAS_THRESHOLD 600   // uprav podľa testovania (0–1023)

// ---- Flame Sensor ----
#define FLAME_PIN 8

// ---- BUZZER ----
#define BUZZER_PIN 2

// ---- FAN MOTOR  ----
#define FAN_PIN 3
int fanSpeed = 0;

// ---- Auto Fan ----
const int HIGH_TEMP = 26;   // zapnúť ventilátor
const int LOW_TEMP  = 25;   // vypnúť ventilátor
bool autoFan = false;

// ---- Blynk slider pre FAN  ----
BLYNK_WRITE(V7) {
  fanSpeed = param.asInt();      
  analogWrite(FAN_PIN, fanSpeed);
}

// ---- Setup ----
void setup() {
  Serial.begin(9600);

  pinMode(FLAME_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);

  analogWrite(FAN_PIN, 0);       

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "64.225.16.22", 8080);
  dht.begin();
}

// ---- Loop ----
void loop() {
  Blynk.run();

  static unsigned long timer = 0;
  if (millis() - timer > 5000) {  // každých 5 sekúnd
    timer = millis();
    // --- DHT11 Senzor ---
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    // ---- AUTO FAN podľa teploty ----
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

    // --- Senzory ---
    int gasValue = analogRead(GAS_PIN);
    float gasPercent = gasValue * 100.0 / 1023.0;
    int flameDetected = !digitalRead(FLAME_PIN);

    // --- Alarm: flame ALEBO vysoký plyn ---
    if (flameDetected || gasValue > GAS_THRESHOLD) {
      tone(BUZZER_PIN, 1000);
    } else {
      noTone(BUZZER_PIN);
    }

    // --- Odoslanie na Blynk ---
    if (!isnan(t)) Blynk.virtualWrite(V0, t);
    if (!isnan(h)) Blynk.virtualWrite(V1, h);
    Blynk.virtualWrite(V3, gasPercent);
    Blynk.virtualWrite(V4, flameDetected);
    Blynk.virtualWrite(V5, flameDetected);
    Blynk.virtualWrite(V7, fanSpeed); 

    // --- Debug do Serial Monitoru ---
    Serial.print("T: "); Serial.print(t);
    Serial.print("C  H: "); Serial.print(h);
    Serial.print("%  Gas: "); Serial.print(gasPercent);
    Serial.print("% (raw "); Serial.print(gasValue); Serial.print(")");
    Serial.print("  Flame: "); Serial.println(flameDetected);
  }
}
