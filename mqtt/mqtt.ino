#include <WiFi.h>
#include <PubSubClient.h>

// Wi-Fi credentials
const char* ssid = "yasin";
const char* password = "yaaz123456";

// MQTT broker details
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;
const char* mqttClientID = "ESP32Client_12345";
const char* mqttTopicSub = "esp32/led/yasin";       // topic to receive LED control
const char* mqttTopicPub = "esp32/switch/yasin";    // topic to publish switch state

WiFiClient espClient;
PubSubClient client(espClient);

// GPIOs
const int SWITCH_PIN = 34;
const int LED_PIN = 2;  // Onboard LED on many ESP32 boards

// Debounce variables
bool lastSwitchReading = HIGH;
bool switchState = HIGH; // Debounced switch state
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup_wifi() {
  Serial.print("Connecting to Wi-Fi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ Wi-Fi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// Handle incoming MQTT messages
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("📥 Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");

  String msg;
  for (unsigned int i = 0; i < length; i++) {
    msg += (char)message[i];
  }
  Serial.println(msg);

  // If message is to control LED
  if (String(topic) == mqttTopicSub) {
    if (msg == "ON") {
      digitalWrite(LED_PIN, HIGH);
      delay(1000);

      Serial.println("💡 LED turned ON");
    } else if (msg == "OFF") {
      digitalWrite(LED_PIN, LOW);
      Serial.println("💡 LED turned OFF");
    }
  }
}

void reconnect_mqtt() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker... ");
    if (client.connect(mqttClientID)) {
      Serial.println("✅ Connected!");
      client.subscribe(mqttTopicSub);  // Subscribe to LED control
    } else {
      Serial.print("❌ Failed, rc=");
      Serial.print(client.state());
      Serial.println(" trying again in 2 seconds...");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(SWITCH_PIN, INPUT); // external pull-up assumed
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Start with LED off

  setup_wifi();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect_mqtt();
  }
  client.loop();

  int reading = digitalRead(SWITCH_PIN);

  // Debounce check
  if (reading != lastSwitchReading) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != switchState) {
      switchState = reading;

      if (switchState == LOW) {
        Serial.println("🔘 Switch Pressed!");
        client.publish(mqttTopicPub, "Switch pressed");
      } else {
        Serial.println("🔼 Switch Released!");
        client.publish(mqttTopicPub, "Switch released");
      }
    }
  }

  lastSwitchReading = reading;
}

