// Este modulo se encargar de realizar la medicion del nivel del tanque
#pragma once

// Todas las variables aqui deben estar en cm 

// Profundidad o altura del tanque
const float TANK_DEEP       = 17;

// Altura del sensor respecto el fondo del tanque
const float SENSOR_HEIGHT   = 17;

float getLevel_cm(float distance_cm);
int getLevelPercent(float level_cm);