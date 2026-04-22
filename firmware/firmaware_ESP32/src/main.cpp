#include <Arduino.h>
// Todos estos includes pueden encontrarlos a la izquierda en la carpeta 'lib'
// Cada archivo de cabecera .h solamente contiene las definiciones de las funciones.
// Las funciones en si mismas estan en los archivos .cpp
#include "serial_comm.h"
#include "wifi_manager.h"
#include "sensors.h"
#include "pins.h"
#include "level.h"
#include "actuators.h"
#include "mqtt_manager.h"

void pusblish_level ();
void publish_pump_state();
void read_serial_command();

void setup() {
    initSerial();   // Inicia la comunicacion serial 
    initMqtt();     // Inicia la conexion MQTT
    initWiFi();     // Inicia la conexion wifi
    initPins();     // Inicia los pines
}

void loop() {

    int publish_time = 1000;
    unsigned long last_publish = 0;

    if (millis() - last_publish >= publish_time) {
        pusblish_level();
        publish_pump_state();
        last_publish = millis();
    }

    if (!sensorCapacitivo() && ordenLlenado) {
        encenderActuadores();
    } else {
        apagarActuadores();
    }
}

void pusblish_level () {
    
    float d = sensorUltrasonico();
    float level_cm = tankLevel(d);
    int mapped_level = map(level_cm, 0, 67, 0, 100);
    
    // Convertir de float a string
    char payload[10];
    dtostrf(mapped_level, 1, 2, payload);  // ancho mínimo 1, 2 decimales

    // Publicar nivel del tanque/contenedor
    mqttClient.publish(TOPIC_NIVEL, 0, false, payload);

}

void publish_pump_state() {
    const char* payload = actuator_s ? "1" : "0";    
    mqttClient.publish(TOPIC_BOMBA, 0, false, payload);
}