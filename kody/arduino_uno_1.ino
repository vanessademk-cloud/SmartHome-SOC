#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// ---------- OLED ----------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ---------- DHT11 ----------
#define DHTPIN A2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Ultrazvukový senzor
#define TRIG_PIN 10
#define ECHO_PIN 11

// Semafor
#define LED_RED    6
#define LED_YELLOW 7
#define LED_GREEN  8

// Spínače s LEDkami
#define BUTTON1_PIN 4
#define LED1_PIN    13

#define BUTTON2_PIN 5
#define LED2_PIN    12

#define BUTTON3_PIN 3
#define LED3_PIN    A1

// Zvukový senzor
#define SOUND_ANALOG A0
#define SOUND_DIGITAL 2
#define SOUND_LED 9

// ---------- SOUND VARIABLES ----------
int data;
bool stavLED = false;
unsigned long poslednyTlesk = 0;

// ---------- OTHER VARIABLES ----------
float duration, distance;

int led1State = LOW;
int lastButton1State;
int currentButton1State;

int led2State = LOW;
int lastButton2State;
int currentButton2State;

int led3State = LOW;
int lastButton3State;
int currentButton3State;

// OLED update
unsigned long previousDisplayMillis = 0;
const unsigned long displayInterval = 2000;

// ---------- SETUP ----------
void setup() {
  Serial.begin(9600);
  Serial.println("UNO Smart Module");

  // Ultrazvukový senzor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Semafor
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  // Spínače s LEDkami
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(LED1_PIN, OUTPUT);

  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(LED2_PIN, OUTPUT);

  pinMode(BUTTON3_PIN, INPUT_PULLUP);
  pinMode(LED3_PIN, OUTPUT);

  // Zvukový senzor
  pinMode(SOUND_LED, OUTPUT);
  digitalWrite(SOUND_LED, LOW);
  attachInterrupt(digitalPinToInterrupt(SOUND_DIGITAL), stopSound, RISING);

  currentButton1State = digitalRead(BUTTON1_PIN);
  currentButton2State = digitalRead(BUTTON2_PIN);
  currentButton3State = digitalRead(BUTTON3_PIN);

  // DHT11
  dht.begin();

  // OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED nenajdeny");
    while (1);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("UNO Smart Module");
  display.println("Start...");
  display.display();
  delay(1500);
}

// ---------- LOOP ----------
void loop() {
  checkClap();
  handleButtons();
  measureDistance();
  updateOLED();

  delay(50);
}

// ---------- Zvukový senzor ----------
void checkClap() {
  data = analogRead(SOUND_ANALOG);

  if (data > 35) {
    if (millis() - poslednyTlesk > 40) {
      stavLED = !stavLED;
      digitalWrite(SOUND_LED, stavLED);

      Serial.print("-> Tlesknutie! LED je ");
      Serial.println(stavLED ? "ZAPNUTA" : "VYPNUTA");

      poslednyTlesk = millis();
    }
  }
}

void stopSound() {
  // do interrupt funkcie radsej nedavaj Serial.println
}

// ---------- Ultrazvukový senzor ----------
void measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration * 0.0343) / 2;

  if (distance > 12) {
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, LOW);
  } 
  else if (distance > 6 && distance < 12) {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_RED, LOW);
  } 
  else if (distance <= 6) {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, HIGH);
  }
}

// ---------- Spínače s LEDkami ----------
void handleButtons() {
  // Button 1
  lastButton1State = currentButton1State;
  currentButton1State = digitalRead(BUTTON1_PIN);

  if (lastButton1State == HIGH && currentButton1State == LOW) {
    led1State = !led1State;
    digitalWrite(LED1_PIN, led1State);
  }

  // Button 2
  lastButton2State = currentButton2State;
  currentButton2State = digitalRead(BUTTON2_PIN);

  if (lastButton2State == HIGH && currentButton2State == LOW) {
    led2State = !led2State;
    digitalWrite(LED2_PIN, led2State);
  }

  // Button 3
  lastButton3State = currentButton3State;
  currentButton3State = digitalRead(BUTTON3_PIN);

  if (lastButton3State == HIGH && currentButton3State == LOW) {
    led3State = !led3State;
    digitalWrite(LED3_PIN, led3State);
  }
}

// ---------- OLED + DHT11 ----------
void updateOLED() {
  if (millis() - previousDisplayMillis < displayInterval) return;
  previousDisplayMillis = millis();

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Smart Home");

  display.setCursor(0, 14);
  if (isnan(temperature) || isnan(humidity)) {
    display.println("DHT chyba");
  } else {
    display.print("Teplota: ");
    display.print(temperature);
    display.println(" C");

    display.print("Vlhkost: ");
    display.print(humidity);
    display.println(" %");
  }

  display.display();
}
