#include "ldr_control.h"

// Define global variables
int ldr1 = 0;
int ldr2 = 0;
int ldr3 = 0;
int ldr4 = 0;

// Define calibrated global variables
float ldr1_calibrated = 0.0;
float ldr2_calibrated = 0.0;
float ldr3_calibrated = 0.0;
float ldr4_calibrated = 0.0;

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
    
    // Update calibrated values
    ldr1_calibrated = normalizeValue(ldr1, LDR1_CAL, LDR1_MIN, LDR1_MAX);
    ldr2_calibrated = normalizeValue(ldr2, LDR2_CAL, LDR2_MIN, LDR2_MAX);
    ldr3_calibrated = normalizeValue(ldr3, LDR3_CAL, LDR3_MIN, LDR3_MAX);
    ldr4_calibrated = normalizeValue(ldr4, LDR4_CAL, LDR4_MIN, LDR4_MAX);
}

float LDRControl::normalizeValue(int rawValue, const int* calPoints, int minVal, int maxVal) {
    // Clamp to extended range
    if (rawValue <= minVal) return 0.0;
    if (rawValue >= maxVal) return 1.0;
    
    // Define normalized positions for 7 calibration points (evenly spaced in output)
    const float outputPoints[7] = {0.0, 0.1667, 0.3333, 0.5, 0.6667, 0.8333, 1.0};
    
    // Check if value is below first calibration point (extrapolate down)
    if (rawValue < calPoints[0]) {
        float slope = (outputPoints[0] - 0.0) / (float)(calPoints[0] - minVal);
        return slope * (rawValue - minVal);
    }
    
    // Check if value is above last calibration point (extrapolate up)
    if (rawValue > calPoints[6]) {
        float slope = (1.0 - outputPoints[6]) / (float)(maxVal - calPoints[6]);
        return outputPoints[6] + slope * (rawValue - calPoints[6]);
    }
    
    // Find which segment the raw value falls into
    for (int i = 0; i < 6; i++) {
        if (rawValue >= calPoints[i] && rawValue <= calPoints[i + 1]) {
            // Linear interpolation between calibration points
            float slope = (outputPoints[i + 1] - outputPoints[i]) / (float)(calPoints[i + 1] - calPoints[i]);
            return outputPoints[i] + slope * (rawValue - calPoints[i]);
        }
    }
    
    // Fallback (should never reach here)
    return constrain((float)(rawValue - minVal) / (float)(maxVal - minVal), 0.0, 1.0);
}
