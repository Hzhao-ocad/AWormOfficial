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

// Global variables for calibrated LDR values (0.0 - 1.0)
extern float ldr1_calibrated;
extern float ldr2_calibrated;
extern float ldr3_calibrated;
extern float ldr4_calibrated;

class LDRControl {
public:
    void init();
    void update();
    
    // Get calibrated light values (0.0 - 1.0)
    float getCalibratedLDR1();
    float getCalibratedLDR2();
    float getCalibratedLDR3();
    float getCalibratedLDR4();
    
private:
    const int numReadings = 10;  // Number of readings for averaging
    
    // 7-point calibration data for each LDR (measured values)
    static const int CALIBRATION_POINTS = 7;
    const int LDR1_CAL[7] = {105, 220, 315, 382, 437, 496, 570};
    const int LDR2_CAL[7] = {328, 503, 634, 736, 812, 890, 976};
    const int LDR3_CAL[7] = {120, 252, 355, 433, 496, 564, 650};
    const int LDR4_CAL[7] = {307, 490, 618, 717, 785, 862, 943};
    
    // Extended min/max values (0-2048 range while maintaining relative differences)
    // Min: all reduced by 105 (LDR1's original min)
    const int LDR1_MIN = 0;
    const int LDR2_MIN = 223;  // 328 - 105
    const int LDR3_MIN = 15;   // 120 - 105
    const int LDR4_MIN = 202;  // 307 - 105
    
    // Max: all increased by 1072 (amount needed for LDR2 to reach 2048)
    const int LDR1_MAX = 1642; // 570 + 1072
    const int LDR2_MAX = 2048; // 976 + 1072
    const int LDR3_MAX = 1722; // 650 + 1072
    const int LDR4_MAX = 2015; // 943 + 1072
    
    // Helper function to normalize raw value using piecewise linear interpolation
    float normalizeValue(int rawValue, const int* calPoints, int minVal, int maxVal);
};

#endif
