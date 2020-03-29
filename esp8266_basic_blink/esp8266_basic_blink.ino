/*
 ESP8266 Blink
 Blink the blue LED on the ESP-12F module
 
 The blue LED on the ESP-12F module is connected to GPIO1 
 (which is also the TXD pin; so we cannot use Serial.print() at the same time)
 
 Note that this sketch uses LED_BUILTIN to find the pin with the internal LED

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

// Set LED_BUILTIN if it is not defined by Arduino framework
#define LED_BUILTIN 1

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output
}

// the loop function runs over and over again forever
void loop()
{
  digitalWrite(LED_BUILTIN, LOW);  // Turn the LED on (Note that LOW is the voltage level
                                   // but actually the LED is on; this is because
                                   // it is acive low on the ESP-12F)
  delay(1000);                     // Wait for a second
  digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off by making the voltage HIGH
  delay(2000);                     // Wait for two seconds (to demonstrate the active low LED)
}
