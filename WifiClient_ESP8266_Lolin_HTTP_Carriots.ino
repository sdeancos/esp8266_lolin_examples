/**
   Envío de Streams a Carriots desde un Lolin32 (NodeMcu v3)
 **/
#include <ESP8266WiFi.h>

// WiFi Setup
const char* ssid     = "SSID_WIFI";
const char* password = "WPA_PASS_WIFI";

// Carriots Setup
IPAddress server(82, 223, 244, 60);
const String apikey = "CARRIOTS_APIKEY";
const String device = "CARRIOTS_ID_DEVELOPER_DEVICE";
String data = "{\"DATA\":\"DATA_LOLIN32\"}";
const short delayTimeStream = 5000;

void conectWifi() {
  // Información del SSID de la WiFi a conectarse
  Serial.println();
  Serial.println("*----------------- [[Trying to Connect to WiFi]] -----------------");

  // Inicio de la conexión
  WiFi.begin(ssid, password);

  // Connectando
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n\n");
}

void printWifiStatus() {

  Serial.println("*----------------- WiFi Status -----------------");
  Serial.println("SSID: " + WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.println("IP Address: " + ip.toString());

  // Fuerza de la Señal WiFi
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");
  Serial.println("*-----------------------------------------------");
}

void sendStream(WiFiClient client) {
  Serial.println("*------------------------ Prepare To Send -----------------");
  Serial.println();
  // Build the data field
  String json = "{\"protocol\":\"v2\",\"device\":\"" + device + "\",\"at\":\"now\",\"data\":" + data + "}";

  String post_request = "POST /streams HTTP/1.1\nHost: api.carriots.com\nAccept: application/json\nContent-Type: application/json\nUser-Agent: Arduino-Carriots\ncarriots.apikey: " +
                        apikey + "\nContent-Length: " + json.length() + "\n\n" + json;
  Serial.println(post_request);

  // Peticion HTTP
  client.println(post_request);

  Serial.println();
  Serial.println("*------------------------ Send -----------------");
}

void setup() {
  // Serial Setup
  Serial.begin(115200);
  delay(50);

  Serial.println("**********************************************************");
  // Inicio Proceso Conexión WiFi

  conectWifi();
  printWifiStatus();
  Serial.println("**********************************************************");
}

void loop() {

  delay(delayTimeStream);

  Serial.println();
  Serial.println("**********************************************************");
  Serial.print("*----------------- [[Connecting to ");
  Serial.println(server.toString() + "]] -----------------");

  // WiFiClient para crear la conexion TCP
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(server, httpPort)) {
    Serial.println("*----------------- Connection Failed -----------------");
    return;
  }

  sendStream(client);

  Serial.println();
  Serial.println("*----------------- [[End Connection]] -----------------");
  Serial.println("\n");
  Serial.println("**********************************************************");
  Serial.println("\n\n\n\n");
}
