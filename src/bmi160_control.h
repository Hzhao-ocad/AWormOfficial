#ifndef BMI160_CONTROL_H
#define BMI160_CONTROL_H

#include <Arduino.h>

// Global variables for orientation (in degrees)
extern float yaw;
extern float pitch;
extern float roll;

class BMI160Control {
public:
    BMI160Control();
    
    // Initialize the BMI160 sensor
    bool init();
    
    // Update sensor readings and calculate orientation
    void update();
    
    // Re-calibrate to adjust for yaw drift
    void recalibrate();
    
    // Get individual values (alternative to using global variables)
    float getYaw() const { return yaw; }
    float getPitch() const { return pitch; }
    float getRoll() const { return roll; }

private:
    float yawOffset;
    float gyroBiasZ;  // Gyroscope Z-axis bias for yaw drift compensation
    unsigned long lastUpdate;
    
    // Sensor fusion helper functions
    void calculateOrientation(float ax, float ay, float az, float gx, float gy, float gz);
    void complementaryFilter(float ax, float ay, float az, float gx, float gy, float gz, float dt);
};

#endif // BMI160_CONTROL_H
