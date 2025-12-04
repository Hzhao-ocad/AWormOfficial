#include <Arduino.h>
#include "Aworm.h"

// WiFi credentials - CHANGE THESE TO YOUR NETWORK
const char* ssid = "Xiaomi_7E62";
const char* password = "12345678";

// Create Aworm instance
Aworm worm;

void setup() {
    Serial.begin(115200);
    
    Serial.println("Initializing AWorm...");
    
    // Initialize all robot subsystems
    if (!worm.begin(ssid, password)) {
        Serial.println("Failed to initialize AWorm!");
        while(1) {
            delay(1000);
        }
    }
    
    Serial.println("AWorm initialized successfully!");
    Serial.print("Web interface available at: http://");
    Serial.println(worm.getIPAddress());
}

void loop() {
    // Main robot update - handles all subsystems
    worm.update();
    
    // Optional: Print sensor data periodically
    // worm.printSensorData();
    
    delay(10); // Update at ~100Hz
}
