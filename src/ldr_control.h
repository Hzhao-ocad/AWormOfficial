#ifndef LDR_CONTROL_H
#define LDR_CONTROL_H

#include <Arduino.h>

// LDR sensor pins
#define LDR_PIN_1 36  // VP
#define LDR_PIN_2 39  // VN
#define LDR_PIN_3 34
#define LDR_PIN_4 35

// Global variables for LDR readings
extern int ldr1;
extern int ldr2;
extern int ldr3;
extern int ldr4;

class LDRControl {
public:
    void init();
    void update();
    
private:
    const int numReadings = 10;  // Number of readings for averaging
};

#endif
