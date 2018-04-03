#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266LLMNR.h>
#include <PubSubClient.h>


const char* ssid = "...";
const char* password = "...";
const char* mqtt_server = "...";


ESP8266WebServer server(80);
ESP8266WebServer secret(81);

WiFiClient espClient;
PubSubClient client(espClient);

void handleRoot() {
  server.send(200, "text/plain", "index page");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if ((char)payload[0] == '1') {
    Serial.println("do something");
  }

}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  setupservers();
  setupmqtt();
}
void setupmqtt() {
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "Clientid";
    clientId += String(random(0xffff) , HEX);
    Serial.println(clientId);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void setupservers() {
  if (MDNS.begin("esp8266") && LLMNR.begin("esp8266")) {
    Serial.println("MDNS and LLMNR responder started");
  }
  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  secret.on("/", []() {
    secret.send(200, "text/plain", "Pssst..!, you are on a secret page");
  });
  server.onNotFound(handleNotFound);
  server.begin();
  secret.begin();
  Serial.println("HTTP server started");
}

void serverhandlers() {
  server.handleClient();
  secret.handleClient();
}

void loop() {
    if (!client.connected()) {
    reconnect();
  }
  serverhandlers();
  client.loop();
}
