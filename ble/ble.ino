#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLEAdvertising.h>

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE Advertising");
  delay(1000);

  // Init BLE
  BLEDevice::init("ESP32_BLE_Test");

  // Create BLE Server (not necessary unless you want services/characteristics)
  BLEServer *pServer = BLEDevice::createServer();

  // Start Advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->start();
  delay(3000);
  Serial.println("BLE Advertising started!");
}

void loop() {
  // nothing
}
