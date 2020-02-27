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

char* message = "Hello world {{card_id}}";
ValueCallback* messageOID;

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

  messageOID = snmp.addStringHandler(".1.3.6.1.4.1.5.101", &message, true);

  // Start the DS18B20 sensors
  sensors.begin();

  // Setup Over The Air Update
  ArduinoOTA.setPort(8266);
  ArduinoOTA.setHostname("esp8266-seduce{{card_id}}");
  ArduinoOTA.setPassword("admin");
  manageOTA();
  ArduinoOTA.begin();

  // Led demo
  {% if card_id == "card1" %}
  pinMode(LED_BUILTIN, OUTPUT);
  {% endif %}
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
  sensor1 = sensors.getTempCByIndex(0)*10;
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