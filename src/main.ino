#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Arduino_SNMP.h>

#include <OneWire.h>
#include <DallasTemperature.h>

// SNMP AGENT
const char* ssid = "Xperia X";
const char* password = "atlexp56";
WiFiUDP udp;
SNMPAgent snmp = SNMPAgent("public");
int temperature = 0;
ValueCallback* temperatureOID;

// DALLAS
const int oneWireBus = 4;     
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);


void setup() {
  Serial.begin(115200);
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

  // give snmp a pointer to the UDP object
  snmp.setUDP(&udp);
  snmp.begin();

  temperatureOID = snmp.addIntegerHandler(".1.3.6.1.4.1.5.1", &temperature, true);

  // snmpget -v 1 -c public 192.168.43.126 1.3.6.1.4.1.5.0

  // Start the DS18B20 sensor
  sensors.begin();

}

void loop() {
  snmp.loop(); // must be called as often as possible
  if (snmp.setOccurred) {
    snmp.sortHandlers();
    snmp.resetSetOccurred();
  }
  updateTemperature();
}

void updateTemperature(){
  sensors.requestTemperatures(); 
  temperature = (int) sensors.getTempCByIndex(0);
  Serial.print(temperature);
  Serial.println("ºC");
  //delay(2000);
  unsigned long start_pause_timestamp = millis();
  while(millis() < start_pause_timestamp + 2000) {
    snmp.loop();
    unsigned long now = millis();
    unsigned long remaining_duration = start_pause_timestamp + 2000 - now;
    unsigned long pause = 100;
    delay(min(remaining_duration, pause));
  } 
}
