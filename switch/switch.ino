const int ledPin = 2;     // GPIO2 connected to LED
const int switchPin = 34; // GPIO34 connected to push-button (with external pull-up)
//const int switchPin4 = 35;
//const int switchPin5 = 5;
//const int switchPin6 = 8;


void setup() {
  pinMode(ledPin, OUTPUT);        // Set LED pin as output
  pinMode(switchPin, INPUT);      // Set switch pin as input (external pull-up required)
  
}
void loop() {
  int switchState = digitalRead(switchPin); // Read the state of the switch
  if (switchState == LOW) {
    digitalWrite(ledPin, HIGH); // Turn ON LED if button is pressed (LOW)
  } else {
    digitalWrite(ledPin, LOW);  // Turn OFF LED if button is released (HIGH)
  }
}