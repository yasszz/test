// GPIO definitions
#define RED_LED     4
#define GREEN_LED   2
#define SW4         35
#define SW5         39
#define SW6         36

// LED states
bool red_on = false;
bool green_on = false;
bool red_was_active = false;

// Timing
unsigned long lastRedToggle = 0;
unsigned long lastGreenToggle = 0;
unsigned long sw5PressTime = 0;

bool sw5Pressed = false;
bool sw6PrevState = HIGH;

void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);

  pinMode(SW4, INPUT);
  pinMode(SW5, INPUT);
  pinMode(SW6, INPUT);

  Serial.begin(115200);
}

// -- Independent input handlers --

void handleSW4() {
  if (digitalRead(SW4) == LOW) {
    red_on = true;
    green_on = false;
    red_was_active = true;
    digitalWrite(GREEN_LED, LOW);  // Make sure green is off
  }
}

void handleSW5(unsigned long now) {
  static bool sw5StatePrev = HIGH;
  bool sw5State = digitalRead(SW5);

  if (sw5State == LOW && sw5StatePrev == HIGH) {
    // Button just pressed
    sw5PressTime = now;
  }

  if (sw5State == HIGH && sw5StatePrev == LOW) {
    // Button just released
    unsigned long pressDuration = now - sw5PressTime;

    if (pressDuration >= 2000) {
      green_on = false;
      digitalWrite(GREEN_LED, LOW);
    } else {
      red_on = false;
      digitalWrite(RED_LED, LOW);
    }
  }

  sw5StatePrev = sw5State;
}


void handleSW6() {
  bool sw6State = digitalRead(SW6);
  if (sw6PrevState == LOW && sw6State == HIGH) {  // Released
    if (!red_on && red_was_active) {
      green_on = true;
      red_was_active = false;
    }
  }
  sw6PrevState = sw6State;
}

// -- Independent LED control based on current states --

void updateRedLED(unsigned long now) {
  if (red_on && now - lastRedToggle >= 50) { // 10Hz
    lastRedToggle = now;
    digitalWrite(RED_LED, !digitalRead(RED_LED));
  }
}

void updateGreenLED(unsigned long now) {
  if (green_on && now - lastGreenToggle >= 500) { // 2Hz
    lastGreenToggle = now;
    digitalWrite(GREEN_LED, !digitalRead(GREEN_LED));
  }
}

// -- Main loop --

void loop() {
  unsigned long now = millis();

  handleSW4();
  handleSW5(now);
  handleSW6();

  updateRedLED(now);
  updateGreenLED(now);
}
