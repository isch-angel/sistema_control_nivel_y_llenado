#include <Arduino.h>
#include "control.h"
#include "sensors.h"
#include "level.h"
#include "mqtt_manager.h"
#include "actuators.h"

// Umbrales para histeresis
const int UMBRAL_ENCENDIDO = 25; 
const int UMBRAL_APAGADO = 20;

// Funcion para actualizar el estado de los actuadores del sistema en funcion del nivel del agua y la orden de llenado
void controlLoop(float level, bool capacitive, bool orden) {
    // Primero, verificamos si tenemos permiso para operar (sensor capacitivo y orden de llenado)
    if (!capacitive && orden) {
        
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

