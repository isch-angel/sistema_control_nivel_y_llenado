#include <Arduino.h>
#include "pins.h"
bool actuator_s = false;

void encenderActuadores() {
  digitalWrite(RELES_PIN, LOW);
  actuator_s = true;
}

void apagarActuadores() {
  digitalWrite(RELES_PIN, HIGH);
  actuator_s = false;
} 