#include <Arduino.h>
#include "sensors.h"
#include "pins.h"

// Esta funcion usa el sensor capacitivo () para determinar si el nivel de agua en el recipiente chico ha sido alcanzado.
// El sensor manda 0 V cuando no detecta nada y 5 V cuando detacta algo.
// Su indicador se enciende cuando no detecta nada y se apaga cuando detecta algo. 
// La funcion retorna un bool "true" cuando el nivel fue alcanzado, "false" cuando el nivel aun no fue alcanzado
bool sensorCapacitivo() {
    return digitalRead(CAPACITIVO_PIN);
}

// Esta funcion usa el sensor ultrasonico (HC-SR04) para determinar la distancia hacia un 
// objetivo, en este caso la superficie del agua en el tanque.
// Retorna la distancia en cm.
float sensorUltrasonico () {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    // Pulso de 10us
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Leer duración del eco
    unsigned long duration_us = pulseInLong(ECHO_PIN, HIGH, 30000);
    
    // Considerar usar termometro para ajustar velocidad del sonido
    // float velocidad_sonido = 0.0331f + (0.0006f * temperatura_c); // cm/us
    // return (duracion_us * velocidad_sonido / 2.0f);
    
    // Si la distancia es "0" probablemente se trate de un error de medicion o el timeout llego a su limite
    // Se devuelve un "-1" indicando el error.
    if (duration_us == 0) return -1;
   
    // Calculo distancia
    return (duration_us * 0.034f / 2.0f);    
}