/*
 *  baisc WIFI connection
 *
 */

#include <ESP8266WiFi.h>

#include "env.h" //include custom header located in same directory

#ifndef WIFI_SSID
#define WIFI_SSID "your-wifi-ssid"    //wifi ssid
#define WIFI_PASSWORD "your-wifi-psk" //wifi password
#endif

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    WiFi.printDiag(Serial);
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
}

