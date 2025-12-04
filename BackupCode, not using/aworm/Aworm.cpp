#include "Aworm.h"

Aworm::Aworm() 
    : currentSpeed(0), lastPrintTime(0), speedSlider(nullptr) {
}

bool Aworm::begin(const char* ssid, const char* password) {
    Serial.println("Starting AWorm initialization...");
    
    // Initialize WiFi and web interface first
    if (!initWiFi(ssid, password)) {
        Serial.println("WiFi initialization failed!");
        return false;
    }
    
    if (!initWebInterface()) {
        Serial.println("Web interface initialization failed!");
        return false;
    }
    
    // Initialize hardware subsystems
    if (!initServos()) {
        Serial.println("Servo initialization failed!");
        return false;
    }
    
    if (!initIMU()) {
        Serial.println("IMU initialization failed!");
        return false;
    }
    
    if (!initLightSensors()) {
        Serial.println("Light sensor initialization failed!");
        return false;
    }
    
    if (!initTouchSensors()) {
        Serial.println("Touch sensor initialization failed!");
        return false;
    }
    
    Serial.println("All subsystems initialized successfully!");
    return true;
}

void Aworm::update() {
    // Update web interface
    GUI.update();
    
    // Update locomotion based on web slider
    updateLocomotion();
    
    // Update all sensor readings
    updateSensors();
}

void Aworm::setSpeed(int speed) {
    currentSpeed = speed;
    servos.move(speed);
}

void Aworm::stop() {
    currentSpeed = 0;
    servos.stop();
}

void Aworm::moveLeftServo(int speed) {
    servos.moveLeftServo(speed);
}

void Aworm::moveRightServo(int speed) {
    servos.moveRightServo(speed);
}

Aworm::Orientation Aworm::getOrientation() const {
    return {yaw, pitch, roll};
}

void Aworm::recalibrateIMU() {
    imu.recalibrate();
}

Aworm::LightSensors Aworm::getLightReadings() const {
    return {ldr1, ldr2, ldr3, ldr4};
}

Aworm::TouchSensors Aworm::getTouchStatus() const {
    return {
        cap1Triggered, 
        cap2Triggered, 
        cap3Triggered, 
        cap4Triggered,
        cap5Triggered, 
        cap6Triggered, 
        cap7Triggered, 
        cap8Triggered
    };
}

String Aworm::getIPAddress() const {
    return GUI.getIP();
}

void Aworm::printSensorData() {
    unsigned long currentTime = millis();
    if (currentTime - lastPrintTime >= 1000) {
        Orientation orient = getOrientation();
        LightSensors light = getLightReadings();
        TouchSensors touch = getTouchStatus();
        
        Serial.print("Yaw: "); Serial.print(orient.yaw);
        Serial.print(" | Pitch: "); Serial.print(orient.pitch);
        Serial.print(" | Roll: "); Serial.println(orient.roll);
        
        Serial.print("LDR1: "); Serial.print(light.ldr1);
        Serial.print(" | LDR2: "); Serial.print(light.ldr2);
        Serial.print(" | LDR3: "); Serial.print(light.ldr3);
        Serial.print(" | LDR4: "); Serial.println(light.ldr4);
        
        Serial.print("CAP1: "); Serial.print(touch.cap1);
        Serial.print(" | CAP2: "); Serial.print(touch.cap2);
        Serial.print(" | CAP3: "); Serial.print(touch.cap3);
        Serial.print(" | CAP4: "); Serial.print(touch.cap4);
        Serial.print(" | CAP5: "); Serial.print(touch.cap5);
        Serial.print(" | CAP6: "); Serial.print(touch.cap6);
        Serial.print(" | CAP7: "); Serial.print(touch.cap7);
        Serial.print(" | CAP8: "); Serial.println(touch.cap8);
        
        lastPrintTime = currentTime;
    }
}

// Private initialization methods
bool Aworm::initWiFi(const char* ssid, const char* password) {
    Serial.println("Connecting to WiFi...");
    if (!GUI.connectWiFi(ssid, password)) {
        Serial.println("WiFi connection failed! Starting AP mode...");
        GUI.startAP("WormControl", "worm1234");
    }
    
    Serial.println("WiFi connected!");
    Serial.print("IP Address: ");
    Serial.println(GUI.getIP());
    
    return true;
}

bool Aworm::initServos() {
    Serial.println("Initializing servos...");
    servos.init();
    Serial.println("Servos initialized!");
    return true;
}

bool Aworm::initIMU() {
    Serial.println("Initializing BMI160...");
    if (!imu.init()) {
        Serial.println("BMI160 initialization failed!");
        return false;
    }
    Serial.println("BMI160 initialized!");
    return true;
}

bool Aworm::initLightSensors() {
    Serial.println("Initializing LDR sensors...");
    lightSensors.init();
    Serial.println("LDR sensors initialized!");
    return true;
}

bool Aworm::initTouchSensors() {
    Serial.println("Initializing CAP1188...");
    initCAP1188();
    Serial.println("CAP1188 initialized!");
    return true;
}

bool Aworm::initWebInterface() {
    GUI.setTitle("AWorm Controller");
    
    // Create and add speed slider to web interface
    speedSlider = new Slider("Speed", 20, 50, -100, 100, 0);
    GUI.addElement(speedSlider);
    
    // Start web server
    GUI.begin();
    
    Serial.println("Web server started!");
    Serial.print("Open http://");
    Serial.print(GUI.getIP());
    Serial.println(" in your browser");
    
    return true;
}

// Private update methods
void Aworm::updateLocomotion() {
    if (speedSlider != nullptr) {
        int sliderSpeed = speedSlider->getIntValue();
        if (sliderSpeed != currentSpeed) {
            setSpeed(sliderSpeed);
            Serial.print("Speed changed to: ");
            Serial.println(sliderSpeed);
        }
    }
}

void Aworm::updateSensors() {
    // Update IMU
    imu.update();
    
    // Update light sensors
    lightSensors.update();
    
    // Update touch sensors
    readCAP1188();
}
