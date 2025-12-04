#ifndef WEBSERVO_H
#define WEBSERVO_H

#include <Arduino.h>
#include <WebGUI.h>

class WebServo {
public:
    void init();
    void addSensorDisplays();
    void updateDisplays();
    
private:
    void addCanvasVisualization();
};

#endif
