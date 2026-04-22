// Este modulo unicamente se encarga de aignar los pines para no perder 
// el control de asginaciones o evitar pines duplciados, e incicializarlos.
#pragma once
#include <Arduino.h>

// ================== ESP32 PINOUT NOTES ==================
// ❌ NO usar (reservados para flash):
// GPIO 6 - 11

// ⚠️ Pines de arranque (usar con cuidado):
// GPIO 0, 2, 4, 5, 12, 15
// -> pueden impedir que el ESP32 arranque correctamente

// ⚠️ Solo entrada (no OUTPUT):
// GPIO 34, 35, 36, 39

// ⚠️ Serial (usar con cuidado):
// GPIO 1 (TX), GPIO 3 (RX)

// ✅ Pines seguros recomendados:
// GPIO 13, 14, 16, 17, 18, 19,
// GPIO 21, 22, 23,
// GPIO 25, 26, 27,
// GPIO 32, 33
// ========================================================

// Sensor ultrasónico
constexpr uint8_t TRIG_PIN = 18;
constexpr uint8_t ECHO_PIN = 19;

// Sensor capacitivo
constexpr uint8_t CAPACITIVO_PIN = 21;

// Relevadores
constexpr uint8_t RELES_PIN = 23;

void initPins();