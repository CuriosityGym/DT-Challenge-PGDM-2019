/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  Control a color gradient on NeoPixel strip using a slider!

  For this example you need NeoPixel library:
    https://github.com/adafruit/Adafruit_NeoPixel

  App project setup:
    Slider widget (0...500) on V1
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_NeoPixel.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "3ZwGzfRpoQ1K-BgEalNlc16DAvc-_mkt";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Rahul V V N";
char pass[] = "rahul123";

#define PIN 4

Adafruit_NeoPixel strip = Adafruit_NeoPixel(3, PIN, NEO_GRB + NEO_KHZ800);

BlynkTimer timer;

WidgetRTC rtc;
int motor=13;
int buzzer=12;
String hours = "";
String minutes = "";
String date ="";
String monthNum="";
int h,m,d,mnth=0;
int monthValue=0;
int dateValue=0;
int hourValue=0;
int minuteValue=0;
int boxNum=0;

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
unsigned long previousMillis1 = 0;
unsigned long interval = 120000;
int previousPinValue = 0;
int pinValue;
boolean set = true;
boolean check = true;
boolean buzzerHigh = false;
boolean sound = false;
int buzzerCount = 50;
int buzzerInterval = 500;
int i = 0;

bool timerStarted = true;
unsigned long startTimer = 0;
// Digital clock display of the time
void clockDisplay()
{
  // You can call hour(), minute(), ... at any time
  // Please see Time library examples for details

  String currentTime = String(hour()) + ":" + minute() + ":" + second();
  String currentDate = String(day()) + " " + month() + " " + year();
  Serial.print("Current time: ");
  Serial.print(currentTime);
  Serial.print(" ");
  Serial.print(currentDate);
  Serial.println();
  h =hour();
  m = minute();
  d = day();
  mnth = month(); 

  //if(h>12) h=h/12;
  //if(h==0) h=12;
  if(hourValue == h && minuteValue == m && dateValue == d && monthValue == mnth){
    currentMillis = millis();
    sound = true;
    if(boxNum == 0){
     strip.setPixelColor(boxNum, 255,0,0);
     strip.show();
    }
    if(boxNum == 1){
     strip.setPixelColor(boxNum, 0,255,0);
     strip.show();
    }
    if(boxNum == 2){
     strip.setPixelColor(boxNum, 0,0,255);
     strip.show();
    }
    
    //digitalWrite(buzzer,HIGH);
    //digitalWrite(motor, HIGH);  
  }
  else{
    for(int i=0; i<3; i++){
     strip.setPixelColor(boxNum, 0,0,0);
     strip.show(); 
     digitalWrite(motor, LOW);
     digitalWrite(buzzer, LOW);
    } 
  }
  // Send time to the App
  //Blynk.virtualWrite(V1, currentTime);
  // Send date to the App
 // Blynk.virtualWrite(V2, currentDate);
 Serial.print("hour value is: ");
  Serial.println(hourValue);
  Serial.print("month value is: ");
  Serial.println(minuteValue); 
   Serial.print("date value is: ");
  Serial.println(dateValue);
  Serial.print("month value is: ");
  Serial.println(monthValue);
}

BLYNK_WRITE(V1)
{
 boxNum = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("box value is: ");
  Serial.println(boxNum);
  
}

BLYNK_WRITE(V2)
{
  hourValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("hour value is: ");
  Serial.println(hourValue);
}
BLYNK_WRITE(V3)
{
   minuteValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("month value is: ");
  Serial.println(minuteValue);
}
BLYNK_WRITE(V4)
{
  dateValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("date value is: ");
  Serial.println(dateValue);
}

BLYNK_WRITE(V5)
{
  monthValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("month value is: ");
  Serial.println(monthValue);
}
void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  pinMode(motor, OUTPUT);
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  strip.begin();
  strip.show();
  rtc.begin();
  for(int i=0; i<3; i++){
    strip.setPixelColor(i, 0,255,0);
     strip.show();
  }
  delay(1000);
  for(int i=0; i<3; i++){
    strip.setPixelColor(i, 0,0,0);
    strip.show();
  }
  delay(1000);
  // Other Time library functions can be used, like:
  //   timeStatus(), setSyncInterval(interval)...
  // Read more: http://www.pjrc.com/teensy/td_libs_Time.html

  // Display digital clock every 10 seconds
  timer.setInterval(500L, clockDisplay);
}

void loop()
{
  Blynk.run();
  timer.run();
 /* if (previousPinValue == pinValue)
    {
      currentMillis = millis();
      if (check == true)
      {
        previousMillis = currentMillis;
        check = false;
      }
*/

    if (sound == true)
      {
        if ((currentMillis - previousMillis1 < buzzerInterval) && (i < buzzerCount) && (buzzerHigh == true))
        {
          digitalWrite(buzzer, HIGH);
           digitalWrite(motor, HIGH);
        }
        if ((currentMillis - previousMillis1 > buzzerInterval) && (buzzerHigh == true))
        {
          i++;
          buzzerHigh = false;
          previousMillis1 = currentMillis;
        }

        if ((currentMillis - previousMillis1 <  buzzerInterval) && (i < buzzerCount) && (buzzerHigh == false))
        {
          digitalWrite(buzzer, LOW);
           digitalWrite(motor, LOW);
        }
        if ((currentMillis - previousMillis1 > buzzerInterval) && (buzzerHigh == false))
        {
          i++;
          buzzerHigh = true;
          previousMillis1 = currentMillis;
        }
        if (i == 50) {sound = false;i=0;}
      }
   }
//  }
 // }
//}
/*
void alertUser(){
  if(currentMillis - millis() > 750){
    digitalWrite(buzzerPin, LOW);
    
  }
}
*/
