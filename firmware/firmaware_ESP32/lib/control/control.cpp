#include <Arduino.h>
#include "control.h"
#include "sensors.h"
#include "level.h"
#include "mqtt_manager.h"

void controlLoop(float level, bool capacitive, bool orden) {
    // Primero, verificamos si tenemos permiso para operar (sensor capacitivo y orden de llenado)
    if (!sensorCapacitivo() && getOrdenLlenado()) {
        
        // Si el nivel sube arriba de 25, encendemos
        if (level > UMBRAL_ENCENDIDO) {
            encenderActuadores();
        } 
        // Si el nivel baja de 20, apagamos
        else if (level < UMBRAL_APAGADO) {
            apagarActuadores();
        }
        // NOTA CLAVE: Si el nivel está entre 20 y 25, no hay ningún "else". 
        // Esto significa que el ESP32 simplemente no hace nada y la bomba 
        // se queda en el estado en el que ya estaba (ya sea prendida o apagada).

    } else {
        // Si el capacitivo se activa o nos quitan la orden de llenado, 
        // apagamos todo por seguridad sin importar el nivel.
        apagarActuadores();
    }
}

