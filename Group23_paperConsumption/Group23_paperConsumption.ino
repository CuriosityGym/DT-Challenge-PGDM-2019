/*
 Example using the SparkFun HX711 breakout board with a scale
 By: Nathan Seidle
 SparkFun Electronics
 Date: November 19th, 2014
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

 This is the calibration sketch. Use it to determine the calibration_factor that the main example uses. It also
 outputs the zero_factor useful for projects that have a permanent mass on the scale in between power cycles.

 Setup your scale and start the sketch WITHOUT a weight on the scale
 Once readings are displayed place the weight on the scale
 Press +/- or a/z to adjust the calibration_factor until the output readings match the known weight
 Use this calibration_factor on the example sketch

 This example assumes pounds (lbs). If you prefer kilograms, change the Serial.print(" lbs"); line to kg. The
 calibration factor will be significantly different but it will be linearly related to lbs (1 lbs = 0.453592 kg).

 Your calibration factor may be very positive or very negative. It all depends on the setup of your scale system
 and the direction the sensors deflect from zero state
 This example code uses bogde's excellent library: https://github.com/bogde/HX711
 bogde's library is released under a GNU GENERAL PUBLIC LICENSE
 Arduino pin 2 -> HX711 CLK
 3 -> DOUT
 5V -> VCC
 GND -> GND

 Most any pin on the Arduino Uno will be compatible with DOUT/CLK.

 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.

*/

// Libraries
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "HX711.h"
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define DOUT  12
#define CLK  14
#define kgToLbs 2.2046226218 
HX711 scale;

float weightLBS;
float weightKG;
float weightGRAMS;
float prevWeightKG;
float calibration_factor = -305000; //-7050 worked for my 440lb max scale setup
unsigned long currentMillis = 0;
int interval = 15000;
int buzzer = 13;


U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display

// WiFi parameters
#define WLAN_SSID       "Redmi-23"
#define WLAN_PASS       "password"

// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "ritesh_gupta"
#define AIO_KEY         "2d32996096e74232b8a9e3b231a461d6"  // Obtained from account info on io.adafruit.com


WiFiClient client;
 
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
 
/****************************** Feeds ***************************************/
 
Adafruit_MQTT_Publish paper_weight = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/paper_weight");

void setup() {
  Serial.begin(9600);
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");
  pinMode(buzzer, OUTPUT); 
  scale.begin(DOUT, CLK);
  scale.set_scale();
  scale.tare(); //Reset the scale to 0
  
  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
  Serial.print(F("Connecting to "));
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();

  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());

  // connect to adafruit io
  mqtt.connect();
  u8g2.begin();  
}

void loop() {
  // ping adafruit io a few times to make sure we remain connected
  if(! mqtt.ping(3)) {
    // reconnect to adafruit io
    if(! mqtt.connected())
      mqtt.connect();
  }
  scale.set_scale(calibration_factor); //Adjust to this calibration factor
   weightLBS = scale.get_units();
  weightKG = weightLBS / kgToLbs;
  weightGRAMS = weightKG*1000;
  Serial.print("Weight: ");
  Serial.print(scale.get_units(), 3); //scale.get_units() returns a float
  Serial.print("  "); //You can change this to kg but you'll need to refactor the calibration_factor
  Serial.print(weightKG);
  Serial.print(" KG");
  Serial.print("  ");
  Serial.print(weightGRAMS);
  Serial.print(" Gram");
  Serial.println();

  if(millis() - currentMillis > interval){
  
  if(weightKG != prevWeightKG){
    prevWeightKG = weightKG;
   if (! paper_weight.publish(weightKG))
    Serial.println(F("Failed to publish Door State"));
   else
    Serial.println(F("Door State published!"));
  }  
  currentMillis = millis();  
  }
  if(weightKG > 9.00){
    Serial.println("weight exceeded, remove weight");
    for(int i=0; i<15; i++){
      digitalWrite(buzzer, LOW);
      delay(1000);
      digitalWrite(buzzer, HIGH);
      delay(1000);
    }
  }
   u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_inb30_mn);
    char wght[5];
    String wieghtString = String(weightKG);
    wieghtString.toCharArray(wght,5);
    u8g2.drawStr(0,40,wght);
    u8g2.setFont(u8g2_font_timB14_tr);
    u8g2.drawStr(100,40,"KG");
  } while ( u8g2.nextPage() );
}
