#include "cap1188_control.h"
#include <Wire.h>

// CAP1188 I2C address (default is 0x29, can be 0x28, 0x2A, 0x2B depending on ADDR pin)
#define CAP1188_I2C_ADDR 0x29

// CAP1188 Register addresses
#define CAP1188_MAIN_CONTROL 0x00
#define CAP1188_INPUT_STATUS 0x03
#define CAP1188_SENSITIVITY 0x1F
#define CAP1188_MULTI_TOUCH 0x2A

// Global booleans for each capacitive sensor
bool cap1Triggered = false;
bool cap2Triggered = false;
bool cap3Triggered = false;
bool cap4Triggered = false;
bool cap5Triggered = false;
bool cap6Triggered = false;
bool cap7Triggered = false;
bool cap8Triggered = false;

// Write to CAP1188 register
void writeCAP1188Register(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(CAP1188_I2C_ADDR);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

// Read from CAP1188 register
uint8_t readCAP1188Register(uint8_t reg) {
    Wire.beginTransmission(CAP1188_I2C_ADDR);
    Wire.write(reg);
    Wire.endTransmission();
    
    Wire.requestFrom(CAP1188_I2C_ADDR, 1);
    if (Wire.available()) {
        return Wire.read();
    }
    return 0;
}

void initCAP1188() {
    Wire.begin();
    
    // Allow multiple touches
    writeCAP1188Register(CAP1188_MULTI_TOUCH, 0xFF);
    
    // Set sensitivity (0x00 = most sensitive, 0x7F = least sensitive)
    // Default middle sensitivity
    writeCAP1188Register(CAP1188_SENSITIVITY, 0x40);
    
    // Clear interrupt
    writeCAP1188Register(CAP1188_MAIN_CONTROL, 0x00);
    
    // Initialize all boolean states to false
    cap1Triggered = false;
    cap2Triggered = false;
    cap3Triggered = false;
    cap4Triggered = false;
    cap5Triggered = false;
    cap6Triggered = false;
    cap7Triggered = false;
    cap8Triggered = false;
}

void readCAP1188() {
    // Read the input status register
    uint8_t touched = readCAP1188Register(CAP1188_INPUT_STATUS);
    
    // Update each boolean based on bit status
    cap1Triggered = (touched & 0x01) != 0;
    cap2Triggered = (touched & 0x02) != 0;
    cap3Triggered = (touched & 0x04) != 0;
    cap4Triggered = (touched & 0x08) != 0;
    cap5Triggered = (touched & 0x10) != 0;
    cap6Triggered = (touched & 0x20) != 0;
    cap7Triggered = (touched & 0x40) != 0;
    cap8Triggered = (touched & 0x80) != 0;
    
    // Clear the interrupt by reading the status
    writeCAP1188Register(CAP1188_MAIN_CONTROL, 0x00);
}

uint8_t getCAP1188Status() {
    return readCAP1188Register(CAP1188_INPUT_STATUS);
}
