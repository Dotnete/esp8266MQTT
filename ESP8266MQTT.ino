#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WIFI parameters 
const char* SSID = "YOUR_WIFI_SSID";
const char* PASSWORD = "YOUR_WIFI_PASSWORD";

// MQTT Config
const char* BROKER_MQTT = "YOUR_MQTT_IP"; // MQTT Broker IP 
int BROKER_PORT = YOUR_MQTT_PORT;
WiFiClient espClient;
PubSubClient MQTT(espClient); // Instanciate Client MQTT

void setup() {
  initPins();
  initSerial();
  initWiFi();
  initMQTT();
}

void initPins() {
  pinMode(2, OUTPUT); //GPIO2
  digitalWrite(2, 1);
}

void initSerial() {
  Serial.begin(115200);
}

void initWiFi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting: ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD); // Wifi Connect
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print(SSID);
  Serial.println(" | IP ");
  Serial.println(WiFi.localIP());
}

// MQTT Broker connection
void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(mqtt_callback);
}

// Receive messages
void mqtt_callback(char* topic, byte* payload, unsigned int length) {

  String message;
  for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    message += c;
  }
  Serial.print("Topic ");
  Serial.print(topic);
  Serial.print(" | ");
  Serial.println(message);

  if (message == "1") {
    digitalWrite(2, 1);
    } else {
      digitalWrite(2, 0);
    }
    message = "";
    Serial.println();
    Serial.flush();
  }

  void reconnectMQTT() {
    while (!MQTT.connected()) {
      Serial.print("Trying to connect with Broker MQTT: ");
      Serial.println(BROKER_MQTT);
      if (MQTT.connect("","YOUR_MQTT_USER","YOUR_MQTT_PASSWORD")) {
        Serial.println("Connected");
      MQTT.subscribe("YOUR_TOPIC"); // Assign the topic
      } else {
        Serial.println("Failed connection");
        Serial.println("Trying to reconnect in 2 seconds");
        delay(2000);
      }
    }
  }

  void recconectWiFi() {
    while (WiFi.status() != WL_CONNECTED) {
      delay(100);
      Serial.print(".");
    }
  }

  void loop() {
    if (!MQTT.connected()) {
    reconnectMQTT(); // Retry Worker MQTT Server connection
  }
  recconectWiFi(); // Retry WiFi Network connection
  MQTT.loop();
}
