#include "ldr_control.h"

// Define global variables
int ldr1 = 0;
int ldr2 = 0;
int ldr3 = 0;
int ldr4 = 0;

void LDRControl::init() {
    // Configure ADC pins as input
    pinMode(LDR_PIN_1, INPUT);
    pinMode(LDR_PIN_2, INPUT);
    pinMode(LDR_PIN_3, INPUT);
    pinMode(LDR_PIN_4, INPUT);
    
    // Set ADC resolution to 12-bit (0-4095)
    analogReadResolution(12);
    
    // Set ADC attenuation to 11dB (0-3.3V range)
    analogSetAttenuation(ADC_11db);
    
    Serial.println("LDR sensors initialized");
}

void LDRControl::update() {
    // Read and average multiple samples for stability
    int sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;
    
    for (int i = 0; i < numReadings; i++) {
        sum1 += analogRead(LDR_PIN_1);
        sum2 += analogRead(LDR_PIN_2);
        sum3 += analogRead(LDR_PIN_3);
        sum4 += analogRead(LDR_PIN_4);
        delayMicroseconds(100);  // Small delay between readings
    }
    
    // Update global variables with averaged values
    ldr1 = sum1 / numReadings;
    ldr2 = sum2 / numReadings;
    ldr3 = sum3 / numReadings;
    ldr4 = sum4 / numReadings;
}
