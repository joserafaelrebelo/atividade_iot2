#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

// Configurações do Wi-Fi Virtual do Wokwi (gratuito)
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Configurações do SEU Broker MQTT Privado (HiveMQ Cloud)
const char* mqtt_server = ""; // Ex: blabla.hivemq.cloud
const int mqtt_port = 8883; // Porta para conexão segura TLS/SSL
const char* mqtt_user = ""; // Usuário criado no HiveMQ
const char* mqtt_pass = "";   // Senha criada no HiveMQ

// Mapeamento dos Pinos do Circuito
const int BTN_TAG_1 = 12; // Simula leitura RFID: Furadeira
const int BTN_TAG_2 = 14; // Simula leitura RFID: Multimetro
const int POT_PESO  = 34; // Simula sensor de peso HX711 (Prateleira)
const int LED_TRAVA = 2;  // Simula trava magnética do armário

// Variáveis de controle de estado
int lastState1 = HIGH;
int lastState2 = HIGH;
int ultimoPesoEnviado = -1;
unsigned long ultimoEnvioPeso = 0;

// IMPORTANTE: Uso do WiFiClientSecure para conexão criptografada (exigência do HiveMQ Cloud)
WiFiClientSecure espClient;
PubSubClient client(espClient);

// Função de Callback para receber comandos MQTT (Backend -> ESP32)
void callback(char* topic, byte* payload, unsigned int length) {
  String mensagem = "";
  for (int i = 0; i < length; i++) {
    mensagem += (char)payload[i];
  }
  
  Serial.println("\n[MQTT Rx] Comando recebido no tópico: " + String(topic));
  Serial.println("[MQTT Rx] Payload: " + mensagem);

  // Se o tópico for de controle da trava magnética
  if (String(topic) == "smartlab/armario1/trava/cmd") {
    if (mensagem.indexOf("UNLOCK") >= 0 || mensagem.indexOf("OPEN") >= 0) {
      digitalWrite(LED_TRAVA, HIGH);
      Serial.println(">>> TRAVA LIBERADA (LED ACESO) <<<");
    } else if (mensagem.indexOf("LOCK") >= 0 || mensagem.indexOf("CLOSE") >= 0) {
      digitalWrite(LED_TRAVA, LOW);
      Serial.println(">>> TRAVA BLOQUEADA (LED APAGADO) <<<");
    }
  }
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando ao Wi-Fi virtual: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWi-Fi Conectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Ignora a verificação de certificado SSL para facilitar a simulação
  espClient.setInsecure(); 
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao Broker MQTT (" + String(mqtt_server) + ")... ");
    
    // ID único para a conexão MQTT
    String clientId = "ESP32_SmartLab_Wokwi_";
    clientId += String(random(0xffff), HEX);
    
    // Conecta passando o ClientID, Usuário e Senha
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("Conectado com sucesso!");
      // Inscreve no tópico para receber comandos do Dashboard/Backend
      client.subscribe("smartlab/armario1/trava/cmd");
      Serial.println("Inscrito no tópico: smartlab/armario1/trava/cmd");
    } else {
      Serial.print("Falha na conexão. Código de erro: ");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 3 segundos...");
      delay(3000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  pinMode(BTN_TAG_1, INPUT_PULLUP);
  pinMode(BTN_TAG_2, INPUT_PULLUP);
  pinMode(LED_TRAVA, OUTPUT);
  digitalWrite(LED_TRAVA, LOW);
  
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // 1. LEITURA SIMULADA DO RFID (FURADEIRA - TAG 1)
  int currentState1 = digitalRead(BTN_TAG_1);
  if (lastState1 == HIGH && currentState1 == LOW) {
    String payload = "{\"timestamp\":\"2026-07-27T21:00:00Z\",\"device_id\":\"ESP32_Wokwi\",\"tag_uid\":\"E280689400000001\",\"item\":\"Furadeira\",\"action\":\"CHECK_OUT\"}";
    client.publish("smartlab/armario1/rfid/evento", payload.c_str());
    Serial.println("\n[MQTT Tx] Evento RFID Publicado: Retirada de Furadeira");
    delay(300); // Debounce
  }
  lastState1 = currentState1;

  // 2. LEITURA SIMULADA DO RFID (MULTIMETRO - TAG 2)
  int currentState2 = digitalRead(BTN_TAG_2);
  if (lastState2 == HIGH && currentState2 == LOW) {
    String payload = "{\"timestamp\":\"2026-07-27T21:00:00Z\",\"device_id\":\"ESP32_Wokwi\",\"tag_uid\":\"E280689400000002\",\"item\":\"Multimetro\",\"action\":\"CHECK_IN\"}";
    client.publish("smartlab/armario1/rfid/evento", payload.c_str());
    Serial.println("\n[MQTT Tx] Evento RFID Publicado: Devolução de Multimetro");
    delay(300); // Debounce
  }
  lastState2 = currentState2;

  // 3. LEITURA CONTÍNUA DO SENSOR DE PESO (POTENCIÔMETRO) - A cada 3 segundos
  if (millis() - ultimoEnvioPeso > 3000) {
    ultimoEnvioPeso = millis();
    int valorAnalogico = analogRead(POT_PESO);
    // Mapeia valor analógico (0-4095) para gramas equivalentes (0 a 5000g)
    int pesoGramas = map(valorAnalogico, 0, 4095, 0, 5000);
    
    // Envia apenas se houver variação significativa (maior que 50g)
    if (abs(pesoGramas - ultimoPesoEnviado) > 50) {
      ultimoPesoEnviado = pesoGramas;
      String payloadPeso = "{\"timestamp\":\"2026-07-27T21:00:00Z\",\"device_id\":\"ESP32_Wokwi\",\"weight_g\":" + String(pesoGramas) + "}";
      client.publish("smartlab/prateleira1/telemetria", payloadPeso.c_str());
      Serial.println("\n[MQTT Tx] Telemetria de Peso Publicada: " + String(pesoGramas) + "g");
    }
  }
}
