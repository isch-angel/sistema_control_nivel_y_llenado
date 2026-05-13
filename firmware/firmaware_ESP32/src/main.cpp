#include <Arduino.h>
// Todos estos includes pueden encontrarlos a la izquierda en la carpeta 'lib'
// Cada archivo de cabecera .h solamente contiene las definiciones de las funciones.
// Las funciones en si mismas estan en los archivos .cpp
#include "wifi_manager.h"
#include "sensors.h"
#include "pins.h"
#include "level.h"
#include "actuators.h"
#include "mqtt_manager.h"
#include "control.h"

unsigned long last_publish = 0;
const int publish_time = 1000;

void setup() {
    Serial.begin(115200);
    initWiFi();     // Inicia la conexion wifi
    initMqtt();     // Inicia la conexion MQTT
    initPins();     // Inicia los pines
}

void loop() {
    float d = getDistance();
    float level = getLevel_cm(d);
    int level_percent = getLevelPercent(level);

    bool capacitive = sensorCapacitivo();
    bool orden_state = getOrdenLlenado();

    mqttLoop(last_publish, publish_time, level_percent);
    controlLoop(level_percent, capacitive, orden_state);
}
