#include <WiFi.h>
#include <WiFiClient.h>

const char* ssid = "Aj";         // Replace with your Wi-Fi SSID
const char* password = "yrbo9055"; // Replace with your Wi-Fi password

void setup() {
  Serial.begin(115200);
  delay(1000);

  // 🔍 Wi-Fi Scan
  Serial.println("Scanning for Wi-Fi networks...");
  int n = WiFi.scanNetworks();
  if (n == 0) {
    Serial.println("No networks found.");
  } else {
    Serial.println("Networks found:");
    for (int i = 0; i < n; ++i) {
      Serial.printf("  %s (Signal: %d dBm)\n", WiFi.SSID(i).c_str(), WiFi.RSSI(i));
      delay(10);
    }
  }

  // 📡 Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // 🌐 Ping a server (simple test)
  WiFiClient client;
  if (client.connect("example.com", 80)) {
    Serial.println("Ping to example.com: ✅ SUCCESS");
    client.stop();
  } else {
    Serial.println("Ping to example.com: ❌ FAILED");
  }
}

void loop() {
  // Nothing in loop
}
