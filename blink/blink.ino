// Define LED pin
const int ledPin = 4; // GPIO4
void setup() {
  // Initialize the LED pin as output
  pinMode(ledPin, OUTPUT);
}
void loop() {
  digitalWrite(ledPin, HIGH); // Turn LED on
  delay(1000);                // Wait for 1 second
  digitalWrite(ledPin, LOW);  // Turn LED off
  delay(1000);                // Wait for 1 second
}