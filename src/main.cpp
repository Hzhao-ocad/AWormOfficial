#include <Arduino.h>
#include <WebGUI.h>

// ESP32-S3 onboard RGB LED pin (WS2812/Neopixel)
#define LED_PIN 48  // GPIO48 for ESP32-S3 onboard RGB LED

// Access Point credentials - ESP32 will create its own WiFi network
const char* AP_SSID = "ESP32-RGB-LED";
const char* AP_PASSWORD = "";  // Empty string = Open WiFi (no password)

// GUI Elements - Sliders for RGB control
Slider redSlider("Red", 20, 50, 0, 255, 0);
Slider greenSlider("Green", 20, 120, 0, 255, 0);
Slider blueSlider("Blue", 20, 190, 0, 255, 0);

// Preset color buttons
Button offButton("OFF", 20, 270, 80);
Button redButton("Red", 110, 270, 80);
Button greenButton("Green", 200, 270, 80);
Button blueButton("Blue", 290, 270, 80);
Button whiteButton("White", 20, 320, 80);
Button purpleButton("Purple", 110, 320, 80);
Button yellowButton("Yellow", 200, 320, 80);
Button cyanButton("Cyan", 290, 320, 80);

// Function to update LED color
void updateLED() {
  int r = redSlider.getIntValue();
  int g = greenSlider.getIntValue();
  int b = blueSlider.getIntValue();
  neopixelWrite(LED_PIN, r, g, b);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("ESP32-S3 RGB LED Web Control");
  
  // Initialize the onboard LED
  pinMode(LED_PIN, OUTPUT);
  neopixelWrite(LED_PIN, 0, 0, 0); // Turn off initially
  
  // Start Access Point mode - ESP32 will create its own WiFi network
  Serial.println("Starting Access Point...");
  GUI.startAP(AP_SSID, AP_PASSWORD);
  
  Serial.println("===============================================");
  Serial.println("   ESP32-S3 RGB LED Controller - AP Mode");
  Serial.println("===============================================");
  Serial.print("   WiFi Network: ");
  Serial.println(AP_SSID);
  Serial.println("   WiFi Password: OPEN (No Password)");
  Serial.println("   IP Address: http://192.168.4.1");
  Serial.println("===============================================");
  Serial.println("1. Connect your phone/computer to the WiFi");
  Serial.println("2. Open browser and go to: http://192.168.4.1");
  Serial.println("===============================================");
  
  // Configure the web interface
  GUI.setTitle("ESP32-S3 RGB LED Controller");
  
  // Add RGB sliders to the interface
  GUI.addElement(&redSlider);
  GUI.addElement(&greenSlider);
  GUI.addElement(&blueSlider);
  
  // Add preset color buttons
  GUI.addElement(&offButton);
  GUI.addElement(&redButton);
  GUI.addElement(&greenButton);
  GUI.addElement(&blueButton);
  GUI.addElement(&whiteButton);
  GUI.addElement(&purpleButton);
  GUI.addElement(&yellowButton);
  GUI.addElement(&cyanButton);
  
  // Start the web server
  GUI.begin();
  
  Serial.println("Setup complete!");
  Serial.println("Control the LED from your browser!");
}

void loop() {
  // Update WebGUI (process web requests)
  GUI.update();
  
  // Handle preset button presses
  if (offButton.wasPressed()) {
    redSlider.setValue(0);
    greenSlider.setValue(0);
    blueSlider.setValue(0);
  }
  
  if (redButton.wasPressed()) {
    redSlider.setValue(255);
    greenSlider.setValue(0);
    blueSlider.setValue(0);
  }
  
  if (greenButton.wasPressed()) {
    redSlider.setValue(0);
    greenSlider.setValue(255);
    blueSlider.setValue(0);
  }
  
  if (blueButton.wasPressed()) {
    redSlider.setValue(0);
    greenSlider.setValue(0);
    blueSlider.setValue(255);
  }
  
  if (whiteButton.wasPressed()) {
    redSlider.setValue(255);
    greenSlider.setValue(255);
    blueSlider.setValue(255);
  }
  
  if (purpleButton.wasPressed()) {
    redSlider.setValue(128);
    greenSlider.setValue(0);
    blueSlider.setValue(128);
  }
  
  if (yellowButton.wasPressed()) {
    redSlider.setValue(255);
    greenSlider.setValue(255);
    blueSlider.setValue(0);
  }
  
  if (cyanButton.wasPressed()) {
    redSlider.setValue(0);
    greenSlider.setValue(255);
    blueSlider.setValue(255);
  }
  
  // Update LED with current slider values
  updateLED();
}