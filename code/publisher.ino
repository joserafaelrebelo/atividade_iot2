#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* topic_pub = "smartlab/renan/lab01/teste01";
const char* client_id = "smartlab_pub_renan_01";

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;
int contador = 0;

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect(client_id)) {
    } else {
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;

    String payload = "{";
    payload += "\"device_id\":\"gateway01\",";
    payload += "\"event_type\":\"item_removed\",";
    payload += "\"item_id\":\"tool_" + String(contador) + "\",";
    payload += "\"location\":\"gaveta_03\",";
    payload += "\"validated_by_weight\":true";
    payload += "}";

    client.publish(topic_pub, payload.c_str(), true);
    Serial.println(payload);
    contador++;
  }
}
