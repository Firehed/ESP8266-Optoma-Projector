#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include "wifi.h"

ESP8266WebServer server(80);

void setup() {
  Serial.begin(19200);
  delay(100);

  connectToWifi();

  server.on("/", [](){
    Serial.readString(); // Discard any buffer
    Serial.println("read");
    String output = Serial.readString();
    server.send(200, "text/plain", output);
  });

  server.on("/select", HTTP_POST, [](){
    String portString = server.arg("port");
    int requestedPort = portString.toInt();
    if (requestedPort < 1 || requestedPort > 8) {
      server.send(400, "text/plain", "invalid port");
      return;
    }
    Serial.println("");
    Serial.readString();
    String command = String("sw i0" + portString);

    Serial.println(command);
    server.send(200, "text/plain", Serial.readString());
  });

  server.begin();
}

void connectToWifi() {
  WiFi.mode(WIFI_STA); // Client only
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("WiFi connected: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWifi();
  }
  server.handleClient();
}
