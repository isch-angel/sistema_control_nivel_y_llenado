#include <Arduino.h>
#include "sensor.h"

constexpr uint8_t CAPACITIVO_PIN = 5;
constexpr uint8_t TRIG_PIN = 18;
constexpr uint8_t ECHO_PIN = 19;

void initSensors() {
    pinMode(CAPACITIVO_PIN, INPUT);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    digitalWrite(TRIG_PIN, LOW);
}

bool sensorCapacitivo() {
    return digitalRead(CAPACITIVO_PIN);
}

// Esta funcion usa el sensor ultrasonico para determinar la distancia a este
// Retorna la distancia en cm
float sensorUltrasonico () {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    // Pulso de 10us
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Leer duración del eco
    unsigned long duracion_us = pulseInLong(ECHO_PIN, HIGH, 30000);
    
    // Considerar usar termometro para ajustar velocidad del sonido
    // float velocidad_sonido = 0.0331f + (0.0006f * temperatura_c); // cm/us
    // return duracion_us * velocidad_sonido / 2.0f;
    
    if (duracion_us == 0) return -1; // o UINT_MAX como centinela
   
    // Calculo distancia
    return duracion_us * 0.034f / 2.0f;    
}