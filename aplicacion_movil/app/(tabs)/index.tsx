import { Buffer } from 'buffer';
global.Buffer = Buffer;

import * as Notifications from 'expo-notifications';
import mqtt from 'mqtt';
import React, { useEffect, useRef, useState } from 'react';
import { LogBox, StyleSheet, Text, TouchableOpacity, View } from 'react-native';

LogBox.ignoreLogs(['Buffer() is deprecated']);

Notifications.setNotificationHandler({
  handleNotification: async () => ({
    shouldShowAlert: true,
    shouldPlaySound: true,
    shouldSetBadge: false,
  }),
});

export default function App() {
  const [porcentaje, setPorcentaje] = useState<number>(0);
  const [status, setStatus] = useState<string>('Iniciando...');
  
  // --- NUEVOS ESTADOS ---
  const [bombaEstado, setBombaEstado] = useState<boolean>(false); // false = 0, true = 1
  const [mqttClient, setMqttClient] = useState<mqtt.MqttClient | null>(null);
  
  const avisoLlenoEnviado = useRef(false);
  const avisoBajoEnviado = useRef(false);

  useEffect(() => {
    solicitarPermisos();

    const options = {
      clientId: 'Cisterna_' + Math.random().toString(16).substring(2, 10),
      path: '/mqtt',
    };

    const client = mqtt.connect('wss://broker.emqx.io:8084/mqtt', options);
    setMqttClient(client); // Guardamos el cliente para usarlo en el botón

    client.on('connect', () => {
      setStatus('Conectado');
      client.subscribe('UACH1/nivel_tanque');
      client.subscribe('UACH1/bomba_estado'); // Suscripción al estado de la bomba
    });

    client.on('message', (topic, message) => {
      const payload = message.toString();

      // Dependiendo del tópico, actualizamos el nivel o la bomba
      if (topic === 'UACH1/nivel_tanque') {
        const valorRecibido = parseInt(payload);
        if (!isNaN(valorRecibido)) {
          const nivelSeguro = Math.max(0, Math.min(100, valorRecibido));
          setPorcentaje(nivelSeguro);
          evaluarNotificaciones(nivelSeguro);
        }
      } 
      else if (topic === 'UACH1/bomba_estado') {
        // Si llega '1' encendemos el LED, si es '0' lo apagamos
        setBombaEstado(payload === '1');
      }
    });

    return () => { if (client) client.end(); };
  }, []);

  async function solicitarPermisos() {
    try {
      const { status } = await Notifications.requestPermissionsAsync();
      if (status !== 'granted') {
        console.log('Permisos de notificación denegados');
      }
    } catch (error) {
      console.warn("No se pudieron pedir permisos en este entorno.");
    }
  }

  const evaluarNotificaciones = (nivel: number) => {
    if (nivel >= 95 && !avisoLlenoEnviado.current) {
      dispararAlerta("Cisterna Llena", "El nivel de agua está al máximo.");
      avisoLlenoEnviado.current = true;
      avisoBajoEnviado.current = false;
    } 
    else if (nivel <= 25 && !avisoBajoEnviado.current) {
      dispararAlerta("Nivel Crítico", "La cisterna está al 25% o menos.");
      avisoBajoEnviado.current = true;
      avisoLlenoEnviado.current = false;
    }
    else if (nivel > 30 && nivel < 90) {
      avisoLlenoEnviado.current = false;
      avisoBajoEnviado.current = false;
    }
  };

  async function dispararAlerta(title: string, body: string) {
    try {
      await Notifications.scheduleNotificationAsync({
        content: { title, body, sound: true, priority: Notifications.AndroidNotificationPriority.HIGH },
        trigger: null,
      });
    } catch (error) {
      console.warn("Aviso de Expo:", error);
    }
  }

  // --- FUNCIÓN PARA EL BOTÓN ---
  const toggleBomba = () => {
    if (mqttClient && status === 'Conectado') {
      // Validamos el estado actual para mandar lo contrario
      const nuevoComando = bombaEstado ? '0' : '1';
      mqttClient.publish('UACH1/ciclo_llenado', nuevoComando);
    }
  };

  const configNivel = () => {
    if (porcentaje <= 25) return { color: '#F44336', msg: 'BAJO', bg: '#FFEBEE' };
    if (porcentaje <= 75) return { color: '#FF9800', msg: 'MEDIO', bg: '#FFF3E0' };
    return { color: '#2196F3', msg: 'ÓPTIMO', bg: '#E3F2FD' };
  };

  const conf = configNivel();

  return (
    <View style={styles.container}>
      <Text style={styles.title}>Monitoreo de Cisterna</Text>
      
      <View style={[styles.msgContainer, { backgroundColor: conf.bg, borderColor: conf.color }]}>
        <Text style={[styles.msgText, { color: conf.color }]}>{conf.msg}</Text>
      </View>

      <View style={styles.cisternaBase}>
        <View style={[styles.agua, { height: `${porcentaje}%`, backgroundColor: conf.color }]} />
      </View>

      <Text style={styles.info}>Nivel de llenado: {porcentaje}%</Text>

      {/* --- NUEVA SECCIÓN: CONTROL DE BOMBA --- */}
      <View style={styles.controlContainer}>
        <View style={styles.estadoBombaContainer}>
          <View style={[styles.led, { backgroundColor: bombaEstado ? '#4CAF50' : '#F44336' }]} />
          <Text style={styles.estadoBombaText}>
            Estado de Bomba: {bombaEstado ? 'ENCENDIDA' : 'APAGADA'}
          </Text>
        </View>

        <TouchableOpacity 
          style={[styles.boton, { backgroundColor: bombaEstado ? '#F44336' : '#4CAF50' }]} 
          onPress={toggleBomba}
          activeOpacity={0.7}
        >
          <Text style={styles.botonText}>
            {bombaEstado ? 'APAGAR BOMBA' : 'ENCENDER BOMBA'}
          </Text>
        </TouchableOpacity>
      </View>
      {/* --------------------------------------- */}

      <Text style={[styles.status, { color: status === 'Conectado' ? 'green' : 'orange' }]}>
        ● {status}
      </Text>
    </View>
  );
}

const styles = StyleSheet.create({
  container: { flex: 1, alignItems: 'center', justifyContent: 'center', backgroundColor: '#fff' },
  title: { fontSize: 24, fontWeight: 'bold', marginBottom: 20 },
  msgContainer: { padding: 10, borderRadius: 10, borderWidth: 2, marginBottom: 20, width: '60%', alignItems: 'center' },
  msgText: { fontWeight: 'bold', fontSize: 18 },
  cisternaBase: { width: 160, height: 280, borderWidth: 4, borderColor: '#333', justifyContent: 'flex-end', backgroundColor: '#eee', overflow: 'hidden' },
  agua: { width: '100%' },
  info: { marginTop: 20, fontSize: 22, fontWeight: 'bold', color: '#444' },
  
  // Estilos nuevos para controles
  controlContainer: { marginTop: 20, alignItems: 'center', width: '80%' },
  estadoBombaContainer: { flexDirection: 'row', alignItems: 'center', marginBottom: 15 },
  led: { width: 16, height: 16, borderRadius: 8, marginRight: 8, borderWidth: 1, borderColor: '#ccc' },
  estadoBombaText: { fontSize: 16, fontWeight: '600', color: '#333' },
  boton: { paddingVertical: 12, paddingHorizontal: 25, borderRadius: 8, elevation: 3, shadowColor: '#000', shadowOffset: { width: 0, height: 2 }, shadowOpacity: 0.2, shadowRadius: 2 },
  botonText: { color: '#fff', fontSize: 16, fontWeight: 'bold' },
  
  status: { marginTop: 25, fontSize: 14 }
});