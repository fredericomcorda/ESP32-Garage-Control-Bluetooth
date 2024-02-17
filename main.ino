#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

int scanTime = 5; // In seconds
int rssiThreshold = -50; // Minimum RSSI value to display
bool deviceFoundWithHighRSSI = false;
const int ledPin = 4;  // LED connected to pin 4


class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getRSSI() > -50) {
      Serial.print("Device found with RSSI > -50: ");
      Serial.print(advertisedDevice.toString().c_str());
      Serial.print(", RSSI: ");
      Serial.println(advertisedDevice.getRSSI());
      deviceFoundWithHighRSSI = true; // Indicate that a device with high RSSI was found
    }
  }
};


void turnOnLED() {
  digitalWrite(ledPin, HIGH); // Turn the LED on
}

void turnOffLED() {
  digitalWrite(ledPin, LOW); // Turn the LED off
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");

  pinMode(ledPin, OUTPUT); // Initialize the LED pin as an output
  turnOffLED(); // Ensure the LED is off at start

  // Create a BLE scanner
  BLEScan* pBLEScan = BLEDevice::getScan();
  // Set the callback function to be called when a device is found
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  // Set active scan to true, which will get more information from devices
  pBLEScan->setActiveScan(true);
  // Set the scan interval and window to be used
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal to the setInterval value
}



void loop() {
  deviceFoundWithHighRSSI = false; // Reset the flag at the start of each scan

  Serial.println("Scanning for BLE devices...");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->start(scanTime, false); // Start a BLE scan
  Serial.println("Scan done!");

  if (deviceFoundWithHighRSSI) {
    Serial.println("High RSSI device found, LED ON");
    turnOnLED(); // Turn on the LED if a device with high RSSI was found
  } else {
    Serial.println("No high RSSI devices found, LED OFF");
    turnOffLED(); // Turn off the LED otherwise
  }

  delay(2000); // Wait a bit before the next scan
}
