// // Este modulo se encarga de la comunicacion con el broker mqtt.
// // De leer los datos publicados en el topic tanque/comandos y de 
// // publicar el estado de los sensores en tanque/sensores.
#pragma once
#include <AsyncMqttClient.h>
#include <AsyncTCP.h>

// Topics
constexpr char TOPIC_COMANDO[]  = "UACH1/ciclo_llenado"; // Suscribirse a comando de Activar o desactivar ciclo de llenado.
constexpr char TOPIC_NIVEL[]    = "UACH1/nivel_tanque";  // Publicar nivel actual de tanque.
constexpr char TOPIC_BOMBA[]    = "UACH1/bomba_estado";  // Publicar estado actual de actuadores.
 
// Broker MQTT 
constexpr char MQTT_SERVER[]    = "broker.emqx.io";
constexpr uint16_t MQTT_PORT    = 1883;
constexpr char MQTT_CLIENT_ID[] = "ESP32_Agua";

// Callbacks functions
void onMqttConnect(bool sessionPresent);
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);

// Cliente accesible desde otros módulos
extern AsyncMqttClient mqttClient;

extern bool ordenLlenado;

void connectToMqtt();
void initMqtt();