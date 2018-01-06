#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "wifi.h"

ESP8266WebServer server(80);

void setup() {
  Serial.begin(9600);
  delay(100);

  connectToWifi();

  server.on("/", [](){
    Serial.println("");
    Serial.readString(); // Discard any buffer
    String command = String("~00124 1");
    Serial.println(command);

    // This will output "OKa"; a=0/1 (off/on)

    server.send(200, "text/plain",  Serial.readString());
  });

  server.on("/power", HTTP_POST, [](){
    String state = server.arg("state");
    String mode;
    if (state == "on") {
        mode = "1";
    } else if (state == "off") {
        mode = "0";
    } else {
      server.send(400, "text/plain", "invalid state");
      return;
    }
    Serial.println("");
    Serial.readString();
    String command = String("~0000 " + mode);

    Serial.println(command);
    server.send(200, "text/plain", Serial.readString());
  });

  server.begin();
}

void connectToWifi() {
  WiFi.hostname(deviceName);
  WiFi.mode(WIFI_STA); // Client only
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("WiFi connected: ");
  Serial.println(WiFi.localIP());

  // Make the device discoverable at `optoma-projector.local`
  if (!MDNS.begin(deviceName)) {
    Serial.println("mDNS responder setup failed");
  }
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWifi();
  }
  server.handleClient();
}
