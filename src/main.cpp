#include <Arduino.h>
#include <WebGUI.h>
#include "servo_control.h"
#include "bmi160_control.h"
#include "ldr_control.h"
#include "cap1188_control.h"
#include "webservo.h"

// WiFi credentials - CHANGE THESE TO YOUR NETWORK
const char* ssid = "Xiaomi_7E61";
const char* password = "12345678";

ServoControl servos;
BMI160Control bmi160;
LDRControl ldrSensors;
WebServo webServo;

// Create speed slider (-100 to 100, default 0)
Slider speedSlider("Speed", 20, 50, -100, 100, 0);

// Create target yaw slider (-180 to 180, default 0)
Slider targetYawSlider("Target Yaw", 20, 120, -180, 180, 0);

// Display for current yaw error
SensorStatus yawErrorDisplay("Yaw Error", 20, 190);

/*
21 SDA I2C Bus (CAP1188, BMI160, Ambient, ToF)
22 SCL I2C Bus
25 DAC/AudioSpeaker via EK1236
26 PWM Servo 1
27 PWM Servo 2
34 ADC LDR 3
35 ADC LDR 4
36 (VP)ADCLDR 1
39 (VN)ADCLDR 2
*/
void setup() {
    Serial.begin(115200);
    
    // Connect to WiFi
    Serial.println("Connecting to WiFi...");
    if (!GUI.connectWiFi(ssid, password)) {
        Serial.println("WiFi connection failed! Starting AP mode...");
        GUI.startAP("WormControl", "worm1234");
    }
    
    Serial.println("");
    Serial.println("WiFi connected!");
    Serial.print("IP Address: ");
    Serial.println(GUI.getIP());
    
    // Configure web interface
    GUI.setTitle("AWorm Controller");
    
    // Add speed slider to web interface
    GUI.addElement(&speedSlider);
    
    // Add target yaw slider and error display
    GUI.addElement(&targetYawSlider);
    GUI.addElement(&yawErrorDisplay);
    
    // Start web server
    GUI.begin();
    
    Serial.println("Web server started!");
    Serial.print("Open http://");
    Serial.print(GUI.getIP());
    Serial.println(" in your browser");
    
    Serial.println("Initializing servos...");
    servos.init();
    Serial.println("Servos initialized!");
    
    Serial.println("Initializing BMI160...");
    if (!bmi160.init()) {
        Serial.println("BMI160 initialization failed!");
    }
    
    Serial.println("Initializing LDR sensors...");
    ldrSensors.init();
    
    Serial.println("Initializing CAP1188...");
    initCAP1188();
    Serial.println("CAP1188 initialized!");
    
    // Initialize web sensor displays and canvas
    webServo.init();
}

void loop() {
    // Handle WebGUI updates
    GUI.update();
    
    // Update servo speed based on web slider
    static int lastSpeed = 0;
    int currentSpeed = speedSlider.getIntValue();
    if (currentSpeed != lastSpeed) {
        servos.move(currentSpeed);
        Serial.print("Speed changed to: ");
        Serial.println(currentSpeed);
        lastSpeed = currentSpeed;
    }
    
    // Target yaw control - turn to reach target direction
    static float targetYaw = 0;
    float newTargetYaw = targetYawSlider.getIntValue();
    if (abs(newTargetYaw - targetYaw) > 0.5) {
        targetYaw = newTargetYaw;
        Serial.print("Target yaw changed to: ");
        Serial.println(targetYaw);
    }
    
    // Calculate yaw error (shortest angle difference)
    float yawError = targetYaw - yaw;
    // Normalize to -180 to 180 range
    while (yawError > 180) yawError -= 360;
    while (yawError < -180) yawError += 360;
    
    // Update yaw error display
    yawErrorDisplay.setValue(yawError, 1);
    
    // Simple proportional control to turn towards target
    // Positive error = turn right, negative error = turn left
    const float kP = 2.0; // Proportional gain (adjust for responsiveness)
    const float deadzone = 2.0; // Don't move if within 2 degrees
    
    if (abs(yawError) > deadzone) {
        int turnSpeed = constrain((int)(kP * yawError), -100, 100);
        servos.move(turnSpeed);
    } else {
        // Stop when close enough to target
        servos.move(0);
    }
    
    // Update BMI160 sensor data
    bmi160.update();
    
    // Update LDR sensor data
    ldrSensors.update();
    
    // Update CAP1188 sensor data
    readCAP1188();
    
    // Update web sensor displays
    webServo.updateDisplays();
    
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint > 1000) {
        Serial.print("Yaw: "); Serial.print(yaw);
        Serial.print(" | Pitch: "); Serial.print(pitch);
        Serial.print(" | Roll: "); Serial.println(roll);
        Serial.print(" | LDR1: "); Serial.print(ldr1_calibrated);
        Serial.print(" | LDR2: "); Serial.print(ldr2_calibrated);
        Serial.print(" | LDR3: "); Serial.print(ldr3_calibrated);
        Serial.print(" | LDR4: "); Serial.println(ldr4_calibrated);
		Serial.print(" | cap1: "); Serial.print(cap1Triggered);
		Serial.print(" | cap2: "); Serial.print(cap2Triggered);
		Serial.print(" | cap3: "); Serial.print(cap3Triggered);
		Serial.print(" | cap4: "); Serial.print(cap4Triggered);
		Serial.print(" | cap5: "); Serial.print(cap5Triggered);
		Serial.print(" | cap6: "); Serial.print(cap6Triggered);
		Serial.print(" | cap7: "); Serial.print(cap7Triggered);
		Serial.print(" | cap8: "); Serial.println(cap8Triggered);
        lastPrint = millis();

	// 	static unsigned long lastCalibrate = 0;
	// 	if (millis() - lastCalibrate > 30000) {
	// 		bmi160.recalibrate();
	// 		Serial.println("BMI160 recalibrated");
	// 		lastCalibrate = millis();
	// 	}
    }
	
    
    // To recalibrate yaw, call: bmi160.recalibrate();
    
    delay(10); // Update at ~100Hz
}
