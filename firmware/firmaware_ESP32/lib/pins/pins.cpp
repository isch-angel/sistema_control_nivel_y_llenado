// pins.cpp
#include "pins.h"

void initPins() {
    // Sensor capacitivo
    pinMode(CAPACITIVO_PIN, INPUT);
    
    // Sensor ultrasonico
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    // Relevadores
    pinMode(RELES_PIN, OUTPUT);
}