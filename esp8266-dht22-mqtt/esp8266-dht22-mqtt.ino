/*
 ESP8266-12F, DHT22, MQTT Publisher
 
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
#include "DHT.h"
#include <PubSubClient.h> // Allows us to connect to, and publish to the MQTT broker

#include "env.h" //include custom header located in same directory

// DHT definitions
#define DHTPIN 4     // what digital pin the DHT22 is conected to
#define DHTTYPE DHT22 // there are multiple kinds of DHT sensors
DHT dht(DHTPIN, DHTTYPE);

#ifndef WIFI_SSID
#define WIFI_SSID "your-wifi-ssid"    //wifi ssid
#define WIFI_PASSWORD "your-wifi-psk" //wifi password
#endif

// MQTT definitions
const char* mqtt_server = "192.168.100.22";
const char* mqtt_topic = "test";
// The client id identifies the ESP8266 device. Think of it a bit like a hostname (Or just a name, like Greg).
const char* clientID = "esp8266";

// Initialise the WiFi and MQTT Client objects
WiFiClient wifiClient;
PubSubClient client(mqtt_server, 1883, wifiClient); // 1883 is the listener port for the Broker

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    WiFi.printDiag(Serial);
  }

  Serial.println("=====================================");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  Serial.println("=====================================");
  // Connect to MQTT Broker
  // client.connect returns a boolean value to let us know if the connection was successful.
  if (client.connect(clientID)) {
    Serial.println("Connected to MQTT Broker!");
  }
  else {
    Serial.println("Connection to MQTT Broker failed...");
  }

  Serial.println("=====================================");
  Serial.println("Device Started");
  Serial.println("Running DHT!");
  Serial.println("-------------------------------------");
}


int timeSinceLastRead = 0;

void loop() {

  // Report every 2 seconds.
  if(timeSinceLastRead > 2000) {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();

    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      timeSinceLastRead = 0;
      return;
    }

    // Compute heat index in Celsius (isFahreheit = false)
    // heat index: feeled temperature based on temp and humi
    float hic = dht.computeHeatIndex(t, h, false);

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C\t");
    Serial.print("Heat index: ");
    Serial.print(hic);
    Serial.println(" *C");

    // convert to values from DHT22 to char, because client.publish requires char
    char h_char[8];
    dtostrf(h, 6, 2, h_char);
    char t_char[8];
    dtostrf(t, 6, 2, t_char);
    
    // PUBLISH to the MQTT Broker (topic = mqtt_topic, defined at the beginning)
    // Here, "Button pressed!" is the Payload, but this could be changed to a sensor reading, for example.
    if (client.publish(mqtt_topic, t_char)) {
      Serial.println("Message sent!");
    }
    // Again, client.publish will return a boolean value depending on whether it succeded or not.
    // If the message failed to send, we will try again, as the connection may have broken.
    else {
      Serial.println("Message failed to send. Reconnecting to MQTT Broker and trying again");
      client.connect(clientID);
      delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
      client.publish(mqtt_topic, t_char);
    }

    timeSinceLastRead = 0;
  }
  delay(100);
  timeSinceLastRead += 100;
}
