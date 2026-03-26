// Control de nivel de agua - Arduino
// Relé de bomba: pin 7 | Relé de válvula: pin 8 | Sensor capacitivo: pin 2

const int PIN_RELE_BOMBA   = 7;
const int PIN_RELE_VALVULA = 8;
const int PIN_SENSOR       = 2;   // HIGH = nivel alcanzado | LOW = sin nivel

bool ordenLlenado = false;

void setup() {
  Serial.begin(9600);

  pinMode(PIN_RELE_BOMBA,   OUTPUT);
  pinMode(PIN_RELE_VALVULA, OUTPUT);
  pinMode(PIN_SENSOR,       INPUT);

  digitalWrite(PIN_RELE_BOMBA,   LOW);
  digitalWrite(PIN_RELE_VALVULA, LOW);
}

void loop() {

  // --- 1. Leer comando serial de LabVIEW ---
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    if (cmd == '1') {
      ordenLlenado = true;
    }
    else if (cmd == '0') {
      ordenLlenado = false;
      apagarActuadores();
    }
  }

  // --- 2. Leer sensor capacitivo ---
  bool nivelAlcanzado = digitalRead(PIN_SENSOR);  // HIGH = lleno | LOW = vacío

  // --- 3. Lógica de control ---
  if (ordenLlenado) {
    if (!nivelAlcanzado) {
      // Orden activa + nivel NO alcanzado → llenar
      encenderActuadores();
    } else {
      // Orden activa + nivel alcanzado → detener
      apagarActuadores();
    }
  }

  // --- 4. Enviar estado del sensor a LabVIEW ---
  // '1' = nivel alcanzado | '0' = sin nivel
  Serial.println(nivelAlcanzado ? "1" : "0");

  delay(200);
}

void encenderActuadores() {
  digitalWrite(PIN_RELE_BOMBA,   HIGH);
  digitalWrite(PIN_RELE_VALVULA, HIGH);
}

void apagarActuadores() {
  digitalWrite(PIN_RELE_BOMBA,   LOW);
  digitalWrite(PIN_RELE_VALVULA, LOW);
}