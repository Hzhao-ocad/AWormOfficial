#ifndef SERVO_CONTROL_H
#define SERVO_CONTROL_H

#include <ESP32Servo.h>

// Servo pins
#define SERVO_LEFT_PIN 26
#define SERVO_RIGHT_PIN 27

// Servo control values for continuous servos
#define SERVO_STOP 90
#define SERVO_FORWARD 0
#define SERVO_BACKWARD 180

class ServoControl {
private:
    Servo leftServo;
    Servo rightServo;
    bool overrideActive;

public:
    void init();
    void move(int speed);  // speed: -100 to 100 (negative = backward, positive = forward)
    void stop();
    
    // Single servo override functions (overruled by move())
    void moveLeftServo(int speed);   // speed: -100 to 100
    void moveRightServo(int speed);  // speed: -100 to 100
};

#endif
