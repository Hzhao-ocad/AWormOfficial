#include "bmi160_control.h"
#include <Wire.h>
#include <BMI160Gen.h>

// Define global orientation variables
float yaw = 0.0;
float pitch = 0.0;
float roll = 0.0;

// BMI160 I2C address when SAO pin is grounded
#define BMI160_I2C_ADDR 0x68

// Complementary filter alpha value (0.0 to 1.0)
// Higher value = trust gyro more, lower value = trust accelerometer more
#define ALPHA 0.96

// Yaw drift threshold - ignore small gyro values as noise/bias (degrees/second)
#define YAW_DEADBAND 0.5

BMI160Control::BMI160Control() : yawOffset(0.0), gyroBiasZ(0.0), lastUpdate(0) {
}

bool BMI160Control::init() {
    // Initialize I2C communication
    Wire.begin(21, 22); // SDA=21, SCL=22
    
    // Initialize BMI160 with I2C address 0x68 (SAO grounded)
    BMI160.begin(BMI160GenClass::I2C_MODE, BMI160_I2C_ADDR);
    
    // Set accelerometer range to ±2g
    BMI160.setAccelerometerRange(2);
    
    // Set gyroscope range to ±250 degrees/second
    BMI160.setGyroRange(250);
    
    // Wait for sensor to stabilize
    delay(100);
    
    // Verify sensor is responding
    uint8_t chipId = BMI160.getDeviceID();
    if (chipId != 0xD1) {
        Serial.print("BMI160 not found! Chip ID: 0x");
        Serial.println(chipId, HEX);
        return false;
    }
    
    Serial.println("BMI160 initialized successfully!");
    Serial.print("Chip ID: 0x");
    Serial.println(chipId, HEX);
    
    // Initialize timing
    lastUpdate = millis();
    
    // Perform initial calibration
    recalibrate();
    
    return true;
}

void BMI160Control::update() {
    int axRaw, ayRaw, azRaw;
    int gxRaw, gyRaw, gzRaw;
    
    // Read accelerometer and gyroscope data
    BMI160.readAccelerometer(axRaw, ayRaw, azRaw);
    BMI160.readGyro(gxRaw, gyRaw, gzRaw);
    
    // Convert raw values to physical units
    // Accelerometer: convert to g (assuming ±2g range)
    float ax = axRaw / 16384.0;
    float ay = ayRaw / 16384.0;
    float az = azRaw / 16384.0;
    
    // Gyroscope: convert to degrees/second (assuming ±250°/s range)
    float gx = gxRaw / 131.0;
    float gy = gyRaw / 131.0;
    float gz = gzRaw / 131.0;
    
    // Calculate time delta
    unsigned long currentTime = millis();
    float dt = (currentTime - lastUpdate) / 1000.0; // Convert to seconds
    lastUpdate = currentTime;
    
    // Prevent issues on first run or overflow
    if (dt > 1.0 || dt <= 0.0) {
        dt = 0.01; // Default to 10ms
    }
    
    // Apply complementary filter for sensor fusion
    complementaryFilter(ax, ay, az, gx, gy, gz, dt);
}

void BMI160Control::complementaryFilter(float ax, float ay, float az, 
                                        float gx, float gy, float gz, float dt) {
    // Calculate pitch and roll from accelerometer (in degrees)
    float accelPitch = atan2(ax, sqrt(ay * ay + az * az)) * 180.0 / PI;
    float accelRoll = atan2(ay, sqrt(ax * ax + az * az)) * 180.0 / PI;
    
    // Integrate gyroscope data (convert to degrees)
    float gyroPitch = pitch + gy * dt;
    float gyroRoll = roll + gx * dt;
    
    // Apply complementary filter for pitch and roll
    pitch = ALPHA * gyroPitch + (1.0 - ALPHA) * accelPitch;
    roll = ALPHA * gyroRoll + (1.0 - ALPHA) * accelRoll;
    
    // Compensate for gyroscope Z-axis bias
    float gzCompensated = gz - gyroBiasZ;
    
    // Apply deadband to filter out noise and small drift
    if (abs(gzCompensated) < YAW_DEADBAND) {
        gzCompensated = 0.0;
    }
    
    // Integrate yaw with compensated gyroscope
    yaw = yaw + gzCompensated * dt;
    
    // Normalize yaw to -180 to 180 degrees
    while (yaw > 180.0) yaw -= 360.0;
    while (yaw < -180.0) yaw += 360.0;
}

void BMI160Control::recalibrate() {
    Serial.println("Calibrating BMI160...");
    Serial.println("Keep the sensor still!");
    
    delay(1000);  // Wait for sensor to stabilize
    
    // Calibrate gyroscope Z-axis bias
    float gzSum = 0.0;
    int samples = 200;  // More samples for better bias estimation
    
    Serial.println("Measuring gyroscope bias...");
    
    for (int i = 0; i < samples; i++) {
        int gxRaw, gyRaw, gzRaw;
        BMI160.readGyro(gxRaw, gyRaw, gzRaw);
        
        // Convert to degrees/second
        float gz = gzRaw / 131.0;
        gzSum += gz;
        
        delay(10);
    }
    
    // Calculate average bias
    gyroBiasZ = gzSum / samples;
    
    // Reset yaw to zero after calibration
    yaw = 0.0;
    
    Serial.println("Calibration complete!");
    Serial.print("Gyro Z bias: ");
    Serial.print(gyroBiasZ);
    Serial.println(" deg/s");
}
