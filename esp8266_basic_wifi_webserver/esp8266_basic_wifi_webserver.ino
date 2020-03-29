/*
 ESP8266 Basic Webserver
 
 Board setup:
 Generic ESP8266 Modul
 Flash Freq: 40 MHZ
 Flash Mode: QIQ
 Flash Size: 51K (no SPIFFS)
 Debug Port: disabled
 Debug Level: keine
 Reset Mothod: nodemcu
 CPU Freq.: 80 MHz
 Upload Speed: 115200
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#ifndef STASSID
#define STASSID "your-ssid"
#define STAPSK "your-password"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

ESP8266WebServer server(80);

void setup()
{
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); //Connect to the WiFi network

  while (WiFi.status() != WL_CONNECTED)
  { //Wait for connection
    delay(500);
    WiFi.printDiag(Serial);
  }

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); //Print the local IP

  server.on("/other", []() { //Define the handling function for the path
    server.send(200, "text / plain", "Other URL");
  });

  server.on("/", handleRootPath); //Associate the handler function to the path

  server.begin(); //Start the server
  Serial.println("Server listening");
}

void loop()
{
  server.handleClient(); //Handling of incoming requests
}

void handleRootPath()
{ //Handler for the rooth path
  server.send(200, "text/plain", "Hello world");
}
