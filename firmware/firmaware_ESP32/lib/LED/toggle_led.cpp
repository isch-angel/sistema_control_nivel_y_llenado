#include <Arduino.h>
#include "toggle_led.h"

constexpr uint8_t LED_PIN = 2;

void initLed() {
    pinMode(LED_PIN, OUTPUT);
}

void toggleLed() {
    digitalWrite(LED_PIN, HIGH);
    delay(50);
    digitalWrite(LED_PIN, LOW);
    delay(50);
    digitalWrite(LED_PIN, HIGH);
    delay(50);
    digitalWrite(LED_PIN, LOW);
    delay(50);
    digitalWrite(LED_PIN, HIGH);
    delay(50);
    digitalWrite(LED_PIN, LOW);
    delay(300);
}