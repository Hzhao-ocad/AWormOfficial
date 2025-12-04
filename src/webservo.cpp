#include "webservo.h"
#include "ldr_control.h"
#include "cap1188_control.h"
#include "bmi160_control.h"

// Create display elements for sensors
SensorStatus ldr1_raw("LDR1 Raw", 20, 350);
SensorStatus ldr2_raw("LDR2 Raw", 20, 400);
SensorStatus ldr3_raw("LDR3 Raw", 20, 450);
SensorStatus ldr4_raw("LDR4 Raw", 20, 500);

SensorStatus ldr1_cal("LDR1 Cal", 250, 350);
SensorStatus ldr2_cal("LDR2 Cal", 250, 400);
SensorStatus ldr3_cal("LDR3 Cal", 250, 450);
SensorStatus ldr4_cal("LDR4 Cal", 250, 500);

SensorStatus yaw_display("Yaw", 480, 350);
SensorStatus pitch_display("Pitch", 480, 400);
SensorStatus roll_display("Roll", 480, 450);

SensorStatus cap1_display("Cap1", 20, 550);
SensorStatus cap2_display("Cap2", 20, 600);
SensorStatus cap3_display("Cap3", 20, 650);
SensorStatus cap4_display("Cap4", 20, 700);
SensorStatus cap5_display("Cap5", 250, 550);
SensorStatus cap6_display("Cap6", 250, 600);
SensorStatus cap7_display("Cap7", 250, 650);
SensorStatus cap8_display("Cap8", 250, 700);

void WebServo::init() {
    Serial.println("Setting up sensor displays for WebGUI...");
    addSensorDisplays();
    addCanvasVisualization();
}

void WebServo::addSensorDisplays() {
    // Add LDR raw value displays
    GUI.addElement(&ldr1_raw);
    GUI.addElement(&ldr2_raw);
    GUI.addElement(&ldr3_raw);
    GUI.addElement(&ldr4_raw);
    
    // Add LDR calibrated value displays
    GUI.addElement(&ldr1_cal);
    GUI.addElement(&ldr2_cal);
    GUI.addElement(&ldr3_cal);
    GUI.addElement(&ldr4_cal);
    
    // Add IMU displays
    GUI.addElement(&yaw_display);
    GUI.addElement(&pitch_display);
    GUI.addElement(&roll_display);
    
    // Add capacitive touch displays
    GUI.addElement(&cap1_display);
    GUI.addElement(&cap2_display);
    GUI.addElement(&cap3_display);
    GUI.addElement(&cap4_display);
    GUI.addElement(&cap5_display);
    GUI.addElement(&cap6_display);
    GUI.addElement(&cap7_display);
    GUI.addElement(&cap8_display);
    
    Serial.println("Sensor displays added to WebGUI");
}

void WebServo::addCanvasVisualization() {
    // Note: WebGUI library doesn't support addHTML() method
    // Canvas drawing would need to be added through custom CSS or by modifying the library
    // For now, we'll skip the canvas visualization
    
    Serial.println("Canvas visualization not supported by WebGUI library");
}

void WebServo::updateDisplays() {
    // Update LDR raw values
    ldr1_raw.setValue(ldr1);
    ldr2_raw.setValue(ldr2);
    ldr3_raw.setValue(ldr3);
    ldr4_raw.setValue(ldr4);
    
    // Update LDR calibrated values
    ldr1_cal.setValue(ldr1_calibrated, 3);
    ldr2_cal.setValue(ldr2_calibrated, 3);
    ldr3_cal.setValue(ldr3_calibrated, 3);
    ldr4_cal.setValue(ldr4_calibrated, 3);
    
    // Update IMU values
    yaw_display.setValue(yaw, 2);
    pitch_display.setValue(pitch, 2);
    roll_display.setValue(roll, 2);
    
    // Update capacitive touch values
    cap1_display.setValue(cap1Triggered);
    cap2_display.setValue(cap2Triggered);
    cap3_display.setValue(cap3Triggered);
    cap4_display.setValue(cap4Triggered);
    cap5_display.setValue(cap5Triggered);
    cap6_display.setValue(cap6Triggered);
    cap7_display.setValue(cap7Triggered);
    cap8_display.setValue(cap8Triggered);
}
