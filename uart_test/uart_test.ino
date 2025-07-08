void setup() {
  Serial.begin(115200);
  Serial.println("UART test started.");
}

void loop() {
  if (Serial.available()) {
    char ch = Serial.read();
    Serial.print("You typed: ");
    Serial.println(ch);
  }
}
