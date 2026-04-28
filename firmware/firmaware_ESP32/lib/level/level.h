// Este modulo se encargar de realizar la medicion del nivel del tanque
#pragma once

// Todas las variables aqui deben estar en cm 

// Profundidad o altura del tanque
const float TANK_DEEP       = 17;

// Altura del sensor respecto el fondo del tanque
const float SENSOR_HEIGHT   = 17;

float tankLevel(float distance_cm);

// 17.5;
// 16.4;