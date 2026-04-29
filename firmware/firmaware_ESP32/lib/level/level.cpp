#include <Arduino.h>
#include "level.h"

/** Esta funcion devuelve el nivel del agua bajo el sensor ultrasonico.
* Primero hay que ajustar las las variables de altura en el level.h
* @param float Distancia del sensor ultrasonico
* @return Devuelve nivel en cm
*/
float getLevel_cm(float distance_cm) {
    float level_cm = SENSOR_HEIGHT - distance_cm;
    return constrain(level_cm, 0, TANK_DEEP);
}

/** Esta funcion devuelve el nivel del agua bajo el sensor ultrasonico.
* @param float Nivel del agua en centimetros
* @return Devuelve nivel en porcentaje
*/
int getLevelPercent(float level_cm) {
    float percent = (level_cm * 100.0f) / TANK_DEEP;
    return constrain((int)percent, 0, 100);
}