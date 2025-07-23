const int ledPin = 13; // Built-in LED pin

void setup() {
  pinMode(ledPin, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Firmware v1.0");
}

void loop() {
  if (Serial.available()) {
    char input = Serial.read();

    if (input == '1') {
      digitalWrite(ledPin, HIGH); // Turn ON LED
      Serial.println("LED ON");
    } 
    else if (input == '0') {
      digitalWrite(ledPin, LOW); // Turn OFF LED
      Serial.println("LED OFF");
    }
 
  }
}
