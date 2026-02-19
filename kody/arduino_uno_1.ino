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
  attachInterrupt(digitalPinToInterrupt(SOUND_DIGITAL), stop, RISING);

  currentButton1State = digitalRead(BUTTON1_PIN);
  currentButton2State = digitalRead(BUTTON2_PIN);
  currentButton3State = digitalRead(BUTTON3_PIN);
}

// ---------- LOOP ----------
void loop() {
  checkClap();          
  handleButtons();      
  measureDistance();    
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

void stop() {
  Serial.println("Detekovan hluk modulem!");
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
  else if (distance > 6 and distance < 12) {
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
