#include <Arduino.h>
#include "level.h"

// Esta funcion devuelve la altura de un objeto colocado bajo el 
// sensor ultrasonico.
// Primero hay que ajustar las las variables de altura en el level.h
float tankLevel(float distance_cm) {
    float level_cm = SENSOR_HEIGHT - distance_cm;
    return constrain(level_cm, 0, 200);
}