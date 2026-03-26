#include <WiFi.h>
#include <PubSubClient.h>

// -------- WIFI --------
const char* ssid = "wifi_docentes";
const char* password = "IReD0BgdY5";

// -------- MQTT --------
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

// -------- PINES --------
const int PIN_RELE_BOMBA_VALVULA = 4;
const int PIN_SENSOR = 2;

bool ordenLlenado = false;

// -------- WIFI --------
void setup_wifi() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

// -------- MQTT CALLBACK --------
void callback(char* topic, byte* message, unsigned int length) {
  String msg;

  for (int i = 0; i < length; i++) {
    msg += (char)message[i];
  }

  // Recibir comando remoto
  if (msg == "0") {
    ordenLlenado = false;
    apagarActuadores();
  }

  if (msg == "1") {
    ordenLlenado = true;
    encenderActuadores();
  }
}

// -------- RECONEXION --------
void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32_Agua")) {
      client.subscribe("tanque/comando");  // aquí llegan órdenes
    } else {
      delay(2000);
    }
  }
}

// -------- SETUP --------
void setup() {
  Serial.begin(115200);

  pinMode(PIN_RELE_BOMBA_VALVULA, OUTPUT);
  pinMode(PIN_SENSOR, INPUT);

  digitalWrite(PIN_RELE_BOMBA_VALVULA, HIGH);

  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

// -------- LOOP --------
void loop() {

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  // --- Leer sensor ---
  bool nivelAlcanzado = digitalRead(PIN_SENSOR);

  // --- Lógica de control ---
  if (ordenLlenado) {
    if (nivelAlcanzado) {
      apagarActuadores();
    } else {
      encenderActuadores();
    }
  }

  // --- Enviar estado por MQTT ---
  client.publish("tanque/nivel", nivelAlcanzado ? "1" : "0");

  delay(200);
}

// -------- FUNCIONES --------
void encenderActuadores() {
  digitalWrite(PIN_RELE_BOMBA_VALVULA, LOW);
}

void apagarActuadores() {
  digitalWrite(PIN_RELE_BOMBA_VALVULA, HIGH);
}