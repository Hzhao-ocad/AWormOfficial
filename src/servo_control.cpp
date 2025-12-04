#include "servo_control.h"

void ServoControl::init() {
    // Allow allocation of all timers for servo library
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);
    
    leftServo.setPeriodHertz(50);    // Standard 50hz servo
    rightServo.setPeriodHertz(50);
    
    leftServo.attach(SERVO_LEFT_PIN, 1000, 2000);
    rightServo.attach(SERVO_RIGHT_PIN, 1000, 2000);
    
    overrideActive = false;
    stop();  // Initialize with servos stopped
}

void ServoControl::move(int speed) {
    // Calling move() cancels any single servo overrides
    overrideActive = false;
    
    // Clamp speed to -100 to 100 range
    if (speed > 100) speed = 100;
    if (speed < -100) speed = -100;
    
    // Map speed (-100 to 100) to servo values (0 to 180)
    int leftValue = map(speed, -100, 100, SERVO_BACKWARD, SERVO_FORWARD);
    int rightValue = map(speed, -100, 100, SERVO_FORWARD, SERVO_BACKWARD);  // Reversed for flipped servo
    
    leftServo.write(leftValue);
    rightServo.write(rightValue);
}

void ServoControl::stop() {
    overrideActive = false;
    leftServo.write(SERVO_STOP);
    rightServo.write(SERVO_STOP);
}

void ServoControl::moveLeftServo(int speed) {
    overrideActive = true;
    
    // Clamp speed to -100 to 100 range
    if (speed > 100) speed = 100;
    if (speed < -100) speed = -100;
    
    // Map speed (-100 to 100) to servo values (0 to 180)
    int leftValue = map(speed, -100, 100, SERVO_BACKWARD, SERVO_FORWARD);
    leftServo.write(leftValue);
}

void ServoControl::moveRightServo(int speed) {
    overrideActive = true;
    
    // Clamp speed to -100 to 100 range
    if (speed > 100) speed = 100;
    if (speed < -100) speed = -100;
    
    // Map speed (-100 to 100) to servo values (0 to 180)
    int rightValue = map(speed, -100, 100, SERVO_FORWARD, SERVO_BACKWARD);  // Reversed for flipped servo
    rightServo.write(rightValue);
}
