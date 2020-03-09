#define BLYNK_PRINT Serial
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = "GKsJ4UFB3hcnYXbRdZoDzg_WvIggBoJ0";
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "DT_LAB";
char pass[] = "fthu@050318";
WidgetMap myMap(V3);
void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);

  // If you want to remove all points:
  //myMap.clear();

  int index = 1;
  float lat = 47.68287;
  float lon = 2.733317;
  myMap.location(index, lat, lon, "value");
}

void loop()
{
  Blynk.run();
}
