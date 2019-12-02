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

int button = 12;


// WiFi parameters
#define WLAN_SSID       "Danish"
#define WLAN_PASS       "drs12345"

// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "aashaygidra"
#define AIO_KEY         "4c0885d69c1847f1ac176a25816db869"  // Obtained from account info on io.adafruit.com

bool wmStarted = true;

WiFiClient client;
 
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
 
/****************************** Feeds ***************************************/
 
Adafruit_MQTT_Publish washingMachine = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/washingMachine");
int pwmValue =1023;
void setup() {
  Serial.begin(9600);
  pinMode(5, OUTPUT);
   pinMode(4, OUTPUT);
   pinMode(0, OUTPUT);
   pinMode(2, OUTPUT);
  pinMode(button, INPUT);
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
}

void loop() {
  // ping adafruit io a few times to make sure we remain connected
  if(! mqtt.ping(3)) {
    // reconnect to adafruit io
    if(! mqtt.connected())
      mqtt.connect();
  }
  if(digitalRead(button) == LOW){
   if (! washingMachine.publish("WIP"))
    Serial.println(F("Failed to publish Door State"));
   else
    Serial.println(F("Door State published!"));
     forward();
    // digitalWrite(motorA, HIGH);
     //digitalWrite(motorB, LOW);
     delay(30000);
     wmStarted = true;
  }
  if(wmStarted == true){
  if (! washingMachine.publish("Standby")){
    Serial.println(F("Failed to publish Door State"));
    wmStarted = false;
  }
   else
    Serial.println(F("Door State published!"));
  
  }
 stopMotor(); 
 delay(3000);
}

void stopMotor(void)
{
    analogWrite(5, 0);
   // analogWrite(4, 0);
}

 
void forward(void)
{
    analogWrite(5, pwmValue);
   // analogWrite(4, pwmValue);
    digitalWrite(0, HIGH);
   // digitalWrite(2, HIGH);
}
