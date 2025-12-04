#ifndef CAP1188_CONTROL_H
#define CAP1188_CONTROL_H

#include <Arduino.h>

// Global booleans for each capacitive sensor (1-8)
extern bool cap1Triggered;
extern bool cap2Triggered;
extern bool cap3Triggered;
extern bool cap4Triggered;
extern bool cap5Triggered;
extern bool cap6Triggered;
extern bool cap7Triggered;
extern bool cap8Triggered;

// Initialize the CAP1188 sensor
void initCAP1188();

// Read all sensor inputs and update global booleans
void readCAP1188();

// Get status of all sensors as a byte (bit 0 = sensor 1, bit 7 = sensor 8)
uint8_t getCAP1188Status();

#endif
