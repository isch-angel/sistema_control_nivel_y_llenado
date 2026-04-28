#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include "mqtt_manager.h"
#include "actuators.h"

// Cliente MQTT asíncrono
AsyncMqttClient mqttClient;

// Timer de FreeRTOS para reintentos de conexión MQTT
static TimerHandle_t mqttReconnectTimer;

// Intervalo de reconexión (en milisegundos)
constexpr unsigned long RECONNECT_INTERVAL_MS = 5000;

// Variable global que representa la orden recibida (ej: activar llenado)
static bool ordenLlenado = false;

bool getOrdenLlenado() {
    return ordenLlenado;
}

void setOrdenLlenado(bool valor) {
    ordenLlenado = valor;
}

/**
 * Inicializa la configuración del cliente MQTT:
 * - Define callbacks (eventos)
 * - Configura servidor y credenciales
 * - Inicializa el timer de reconexión
 */
void initMqtt() {
    mqttClient.onConnect(onMqttConnect);         // Callback al conectar
    mqttClient.onDisconnect(onMqttDisconnect);   // Callback al desconectar
    mqttClient.onMessage(onMqttMessage);         // Callback al recibir mensaje

    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);   // Configura broker MQTT
    mqttClient.setClientId(MQTT_CLIENT_ID);         // ID único del cliente

    initMqttReconnectTimer();  // Inicializa el timer de reconexión
}

/**
 * Callback del timer de reconexión MQTT.
 * Se ejecuta cuando el timer expira.
 * Intenta reconectar solo si WiFi sigue conectado.
 */
void mqttReconnectTimerCallback(TimerHandle_t xTimer) {
    if (WiFi.status() != WL_CONNECTED) return; // Evita reconectar sin WiFi

    Serial.println("Intentando reconectar a MQTT...");
    mqttClient.connect();
}

/**
 * Inicializa el timer de FreeRTOS que se usa para reconectar MQTT.
 * - No es periódico (one-shot)
 * - Se activa manualmente cuando ocurre una desconexión
 */
void initMqttReconnectTimer() {
    mqttReconnectTimer = xTimerCreate(
        "mqttReconnect",                      // Nombre del timer (debug)
        pdMS_TO_TICKS(RECONNECT_INTERVAL_MS), // Periodo en ticks
        pdFALSE,                              // No auto-repetitivo
        nullptr,                              // ID opcional
        mqttReconnectTimerCallback            // Función callback
    );
}

/**
 * Fuerza una conexión al broker MQTT.
 */
void connectToMqtt() {
    Serial.println("Connecting to MQTT...");
    mqttClient.connect();
}

/**
 * Callback ejecutado cuando se pierde la conexión MQTT.
 * Activa el timer para intentar reconectar después de un tiempo.
 */
void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
    Serial.println("Disconnected from MQTT");

    // Inicia el timer de reconexión (no bloqueante)
    xTimerStart(mqttReconnectTimer, 0);
}

/**
 * Callback ejecutado cuando se establece conexión MQTT.
 * - Detiene el timer de reconexión si estaba activo
 * - Se suscribe al tópico de comandos
 */
void onMqttConnect(bool sessionPresent) {
    Serial.println("Connected to MQTT.");

    // Detiene cualquier intento de reconexión pendiente
    xTimerStop(mqttReconnectTimer, 0);

    // Suscripción al tópico donde se reciben comandos
    mqttClient.subscribe(TOPIC_COMANDO, 0);
}

/**
 * Callback que se ejecuta al recibir un mensaje MQTT.
 * 
 * @param topic     Tópico del mensaje
 * @param payload   Contenido del mensaje
 * @param properties Propiedades MQTT
 * @param len       Longitud del payload
 * @param index     Índice del fragmento (para mensajes grandes)
 * @param total     Tamaño total del mensaje
 */
void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {

    // Filtra mensajes que no sean del tópico esperado
    if (strcmp(topic, TOPIC_COMANDO) != 0) return;
    
    // Validación simple del payload:
    // Se espera un solo caracter: '1' (activar) o '0' (desactivar)
    if (len == 1 && payload[0] == '1') {
        setOrdenLlenado(true);
    } else if (len == 1 && payload[0] == '0') {
        setOrdenLlenado(false);
    } else {
        Serial.println("Payload invalido");
    }
}