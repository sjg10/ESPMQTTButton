/*
  ESPMQTT Button

  Connects to WiFi and publishes a sequence of MQTT messages.
  Then enters deep sleep until reset trigger=red e.g. via a button.
  Notifies publishing in progress by illuminating an LED, and an error by blinking that LED (and the system LED).

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "ESP8266WiFi.h"
#include <PubSubClient.h>

// WiFi config
const char* ssid = ""; //Enter SSID
const char* password = ""; //Enter Password

// MQTT Config
const char* mqttServer = ""; //Enter MQTT server IP or domain address
const int mqttPort = 1883; //Enter MQTT port
const char* mqttUser = ""; //Enter MQTT username
const char* mqttPassword = ""; //Enter MQTT password

// Desired MQTT Messages
const char* topics[] = {"tasmota1/cmnd/Power", "tasmota1/cmnd/Power", "tasmota1/cmnd/Power", "tasmota1/cmnd/Power"};
const char* messages[] = {"TOGGLE", "TOGGLE", "TOGGLE", "TOGGLE"};

// Minimum LED lit time in ms for visuals
const int minLEDTime = 1000;

// Pin defs
#define BUILTIN_LED 16
#define LED 5 // D1 on NodeMCU

// Error codes
#define WIFI_ERROR_CODE 1
#define MQTT_CONNECT_ERROR_CODE 2
#define MQTT_PUBLISH_ERROR_CODE 3

// Blink out an error code on the LED indefinitely.
void error(int code) {
  Serial.print("Error code");
  Serial.println(code);
  
  while(1) {
    digitalWrite(LED, LOW);
    digitalWrite(BUILTIN_LED, LOW);
    delay(code * 1000);
    digitalWrite(LED, HIGH);
    digitalWrite(BUILTIN_LED, HIGH);
    delay(code * 1000);
  }
}

// Connect to the WiFi
void wifiSetup(void) {
  WiFi.begin(ssid, password);
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) 
  {
     if(status == WL_NO_SSID_AVAIL || status == WL_CONNECT_FAILED) {
      Serial.print("WIFI status ");
      Serial.println(status);
      error(WIFI_ERROR_CODE);
     }
     delay(500);
     Serial.print("*");
     status =  WiFi.status();
  }
  
  Serial.println("");
  Serial.println("WiFi connection Successful");
  Serial.print("The IP Address of ESP8266 Module is: ");
  Serial.println(WiFi.localIP());// Print the IP address
}

// Connect to the MQTT broker
void mqttSetup(PubSubClient * client) {
  client->setServer(mqttServer, mqttPort);
  while (!client->connected()) {
    Serial.println("Connecting to MQTT...");
  
    if (client->connect("ESP8266Client", mqttUser, mqttPassword )) {
      Serial.println("connected");  
    } else {
      Serial.print("failed with state ");
      Serial.print(client->state());
      error(MQTT_CONNECT_ERROR_CODE);
    }  
  }
}


// Publish the above messages to a connected client
void publishMQTT(PubSubClient * client) {
    size_t message_cnt = sizeof(topics)/sizeof(topics[0]);
    for (int i = 0; i < message_cnt; i++) {
      if(!client->publish(topics[i], messages[i])) error(MQTT_PUBLISH_ERROR_CODE);
    }
    Serial.println("publishing complete");
}

void setup() {
  // initialize pins
  pinMode(LED, OUTPUT);
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(LED, HIGH); // on
  digitalWrite(BUILTIN_LED, HIGH); // off
  
  unsigned long StartTime = millis();
 
  Serial.begin(115200);

  // Connect to WiFi and MQTT
  WiFiClient espClient;
  PubSubClient client(espClient);
  wifiSetup();
  mqttSetup(&client);
  publishMQTT(&client);
  client.disconnect();

  
  // Check how long that took and ensure LED is lit for long enough before turning it off
  unsigned long elapsedTime = StartTime - millis();
  if(elapsedTime < minLEDTime) { delay(minLEDTime - StartTime); }
  digitalWrite(LED, LOW);
  // enter deep sleep until reset button pressed
  ESP.deepSleep(0); 
}

// empty loop
void loop() {
}
