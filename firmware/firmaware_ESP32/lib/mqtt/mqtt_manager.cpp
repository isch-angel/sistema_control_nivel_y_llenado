#include <Arduino.h>
#include <AsyncMqttClient.h>
#include <AsyncTCP.h>
#include "mqtt_manager.h"
#include "actuators.h"

AsyncMqttClient mqttClient;
bool ordenLlenado = false;

void initMqtt() {
    mqttClient.onConnect(onMqttConnect);
    mqttClient.onDisconnect(onMqttDisconnect);
    mqttClient.onMessage(onMqttMessage);
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    mqttClient.setClientId(MQTT_CLIENT_ID);
}

// MQTT connect
void connectToMqtt() {
    Serial.println("Connecting to MQTT...");
    mqttClient.connect();
}

// When MQTT connects
void onMqttConnect(bool sessionPresent) {  
    Serial.println("Connected to MQTT.");
    mqttClient.subscribe(TOPIC_COMANDO, 0);
}

// When MQTT disconnects
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
    Serial.println("Disconnected from MQTT");
}

// When message received
void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
    
    // ¿Qué pasa si recibes otro topic con valor "1"?
    if (strcmp(topic, TOPIC_COMANDO) != 0) return; // Filtra primero
    
    // // Crear un buffer con terminador nulo
    // char msg[len + 1];
    // memcpy(msg, payload, len);
    // msg[len] = '\0';

    // En lugar de atoi, valida el payload directamente
    if (len == 1 && payload[0] == '1') {
        ordenLlenado = true;
    } else if (len == 1 && payload[0] == '0') {
        ordenLlenado = false;
    } else {
        Serial.println("Payload invalido");
    }
}

