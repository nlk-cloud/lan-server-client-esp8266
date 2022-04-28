/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp8266-nodemcu-client-server-wi-fi/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

This program configures the attached ESP8266 board into a server. THe water level sensor is 
attached to A0 pin of this board.  
THus this board reads the voltage on the sensor and it being a server, transmits it to whichever client 
asks for it with http get command or whicever web page with address 192.168.1.2:1880/waterlevel

This is a server side program

*/

// Import required libraries
#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"

unsigned long lastTime = 0;  
unsigned long timerDelay = 30000;
float voltage;
String volts;

// Set your access point network credentials
const char* ssid = "San";
const char* password = "neesa1468";

// Set your Static IP address
IPAddress local_IP(192, 168, 1, 2);

// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


String readLevel()
{
  int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 3.2V):
  voltage = sensorValue * (3.2 / 1023.0);
  // print out the value you read:
  Serial.println(voltage);
  return String(voltage);  
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();

// Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  server.on("/waterlevel", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/plain", readLevel().c_str());
  });
  
  
  server.begin();

}  
  
void loop(){

  if ((millis() - lastTime) > timerDelay) {
    volts = readLevel();
    lastTime = millis();
  }    
  
}
  
  