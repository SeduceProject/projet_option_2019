#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Arduino_SNMP.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <OneWire.h>
#include <DallasTemperature.h>


// WIFI & SNMP AGENT
const char* ssid = "Xperia X";
const char* password = "atlexp56";
WiFiUDP udp;
SNMPAgent snmp = SNMPAgent("public");

// DALLAS
const int oneWireBus = 4;     
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

// SENSORS
float sensor1 = 1;
ValueCallback* sensor1OID;
float sensor2 = 2;
ValueCallback* sensor2OID;
float sensor3 = 3;
ValueCallback* sensor3OID;
float sensor4 = 4;
ValueCallback* sensor4OID;
float sensor5 = 5;
ValueCallback* sensor5OID;
float sensor6 = 6;
ValueCallback* sensor6OID;
float sensor7 = 7;
ValueCallback* sensor7OID;
float sensor8 = 8;
ValueCallback* sensor8OID;
float sensor9 = 9;
ValueCallback* sensor9OID;
float sensor10 = 10;
ValueCallback* sensor10OID;
float sensor11 = 11;
ValueCallback* sensor11OID;
float sensor12 = 12;
ValueCallback* sensor12OID;
float sensor13 = 13;
ValueCallback* sensor13OID;
float sensor14 = 14;
ValueCallback* sensor14OID;
float sensor15 = 15;
ValueCallback* sensor15OID;
float sensor16 = 16;
ValueCallback* sensor16OID;


// ------------------------------------


void setup() {
  Serial.begin(115200);
  Serial.println("");
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

  // Give SNMP a pointer to the UDP object
  snmp.setUDP(&udp);
  snmp.begin();

  sensor1OID = snmp.addFloatHandler(".1.3.6.1.4.1.5.1", &sensor1, true);
  sensor2OID = snmp.addFloatHandler(".1.3.6.1.4.1.5.2", &sensor2, true);
  sensor3OID = snmp.addFloatHandler(".1.3.6.1.4.1.5.3", &sensor3, true);
  sensor4OID = snmp.addFloatHandler(".1.3.6.1.4.1.5.4", &sensor4, true);
  sensor5OID = snmp.addFloatHandler(".1.3.6.1.4.1.5.5", &sensor5, true);
  sensor6OID = snmp.addFloatHandler(".1.3.6.1.4.1.5.6", &sensor6, true);
  sensor7OID = snmp.addFloatHandler(".1.3.6.1.4.1.5.7", &sensor7, true);
  sensor8OID = snmp.addFloatHandler(".1.3.6.1.4.1.5.8", &sensor8, true);
  sensor9OID = snmp.addFloatHandler(".1.3.6.1.4.1.5.9", &sensor9, true);
  sensor10OID = snmp.addFloatHandler(".1.3.6.1.4.1.5.10", &sensor10, true);
  sensor11OID = snmp.addFloatHandler(".1.3.6.1.4.1.5.11", &sensor11, true);
  sensor12OID = snmp.addFloatHandler(".1.3.6.1.4.1.5.12", &sensor12, true);
  sensor13OID = snmp.addFloatHandler(".1.3.6.1.4.1.5.13", &sensor13, true);
  sensor14OID = snmp.addFloatHandler(".1.3.6.1.4.1.5.14", &sensor14, true);
  sensor15OID = snmp.addFloatHandler(".1.3.6.1.4.1.5.15", &sensor15, true);
  sensor16OID = snmp.addFloatHandler(".1.3.6.1.4.1.5.16", &sensor16, true);

  // Start the DS18B20 sensors
  sensors.begin();

  // Setup Over The Air Update
  ArduinoOTA.setPort(8266);
  ArduinoOTA.setHostname("esp8266-seduce1");
  ArduinoOTA.setPassword("admin");
  manageOTA();
  ArduinoOTA.begin();
}


void loop() {
  snmp.loop(); // must be called as often as possible
  updateTemperature();
  doPause();
  ArduinoOTA.handle();
}


void doPause(){
  int delay_get = 2000;  
  unsigned long start_pause_timestamp = millis();
  while(millis() < start_pause_timestamp + delay_get) {
    snmp.loop();
    unsigned long now = millis();
    unsigned long remaining_duration = start_pause_timestamp + delay_get - now;
    unsigned long pause = 10;
    delay(min(remaining_duration, pause));
  } 
}


void updateTemperature(){
  sensors.requestTemperatures(); 
  {% if card_id == "card1" %}
  sensor1 = 10;
  {% else %}
  sensor1 = sensors.getTempCByIndex(0)*10;
  {% endif %}
  sensor2 = sensors.getTempCByIndex(1)*10;
  sensor3 = sensors.getTempCByIndex(2)*10;
  sensor4 = sensors.getTempCByIndex(3)*10;
  sensor5 = sensors.getTempCByIndex(4)*10;
  sensor6 = sensors.getTempCByIndex(5)*10;
  sensor7 = sensors.getTempCByIndex(6)*10;
  sensor8 = sensors.getTempCByIndex(7)*10;
  sensor9 = sensors.getTempCByIndex(8)*10;
  sensor10 = sensors.getTempCByIndex(8)*10;
  sensor11 = sensors.getTempCByIndex(9)*10;
  sensor12 = sensors.getTempCByIndex(10)*10;
  sensor13 = sensors.getTempCByIndex(11)*10;
  sensor13 = sensors.getTempCByIndex(12)*10;
  sensor14 = sensors.getTempCByIndex(13)*10;
  sensor15 = sensors.getTempCByIndex(14)*10;
  sensor16 = sensors.getTempCByIndex(15)*10;
}


void manageOTA(){
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
}