#ifndef AWORM_H
#define AWORM_H

#include <Arduino.h>
#include <WebGUI.h>
#include "../servo_control.h"
#include "../bmi160_control.h"
#include "../ldr_control.h"
#include "../cap1188_control.h"

/**
 * Aworm Class - Main robot controller
 * 
 * Consolidates all robot subsystems:
 * - Locomotion (servo control)
 * - Orientation sensing (BMI160 IMU)
 * - Light sensing (LDR sensors)
 * - Touch sensing (CAP1188 capacitive sensors)
 * - Web interface (WebGUI)
 */
class Aworm {
public:
    // Constructor
    Aworm();
    
    // Initialize all robot subsystems
    bool begin(const char* ssid, const char* password);
    
    // Main update loop - call this in loop()
    void update();
    
    // Locomotion control
    void setSpeed(int speed);  // -100 to 100
    void stop();
    void moveLeftServo(int speed);
    void moveRightServo(int speed);
    
    // Sensor access
    struct Orientation {
        float yaw;
        float pitch;
        float roll;
    };
    Orientation getOrientation() const;
    void recalibrateIMU();
    
    struct LightSensors {
        int ldr1;
        int ldr2;
        int ldr3;
        int ldr4;
    };
    LightSensors getLightReadings() const;
    
    struct TouchSensors {
        bool cap1;
        bool cap2;
        bool cap3;
        bool cap4;
        bool cap5;
        bool cap6;
        bool cap7;
        bool cap8;
    };
    TouchSensors getTouchStatus() const;
    
    // Web interface access
    String getIPAddress() const;
    
    // Debug output
    void printSensorData();
    
private:
    // Subsystem instances
    ServoControl servos;
    BMI160Control imu;
    LDRControl lightSensors;
    
    // Web GUI slider
    Slider* speedSlider;
    
    // Internal state
    int currentSpeed;
    unsigned long lastPrintTime;
    
    // Initialize individual subsystems
    bool initWiFi(const char* ssid, const char* password);
    bool initServos();
    bool initIMU();
    bool initLightSensors();
    bool initTouchSensors();
    bool initWebInterface();
    
    // Update individual subsystems
    void updateLocomotion();
    void updateSensors();
};

#endif // AWORM_H
