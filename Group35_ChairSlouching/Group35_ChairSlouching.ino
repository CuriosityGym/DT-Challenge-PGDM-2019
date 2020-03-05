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

  This example shows how value can be pushed from Arduino to
  the Blynk App.

  NOTE:
  BlynkTimer provides SimpleTimer functionality:
    http://playground.arduino.cc/Code/SimpleTimer

  App project setup:
    Value Display widget attached to Virtual Pin V5
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "XkIS20yAypeQr1Vo6q046lP34FH-_Kz3";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "DT_LAB";
char pass[] = "fthu@050318";
int buttonTop = 14;
int buttonMiddle = 12;
int buttonBottom = 13;
int bTop=0;
int bMiddle = 0;
int bBottom = 0;
int sensorVal=0;
int seat =0;
bool chairOccupied = false;
bool timerStarted = true;
unsigned long startTimer = 0;
int chair =0;

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
unsigned long previousMillis1 = 0;
boolean buzzerHigh = false;
boolean sound = false;
int i = 0;
int buzzerCount = 50;
int buzzerInterval = 500;

int buzzer = 5;
int motor = 15;
bool check = false;
BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  int top,bottom,middle;
  /* if(bTop==1) top = 0;
  else top = 255;
  if(bMiddle==1) middle = 0;
  else middle = 255;
  if(bBottom==1) bottom = 0;
  else bottom = 255;
  if(sensorVal == 0) chair=255;
  else chair = 0;*/
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V0, sensorVal);
  Blynk.virtualWrite(V1, bTop);
  Blynk.virtualWrite(V2, bMiddle);
  Blynk.virtualWrite(V3, bBottom);
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(buttonTop, INPUT);
  pinMode(buttonBottom, INPUT);
  pinMode(buttonMiddle, INPUT);
  pinMode(seat, INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(motor,OUTPUT);
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(500L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
  bTop = digitalRead(buttonTop);
  bMiddle = digitalRead(buttonMiddle);
  bBottom = digitalRead(buttonBottom);
  sensorVal = digitalRead(seat);
 
  //Serial.print(bTop);
  //Serial.print(bMiddle);
  //Serial.println(bBottom);
  if(sensorVal == 0){
    chairOccupied = true;
  }
  else{
    chairOccupied = false;
  }
 
  if(chairOccupied){
     if(bTop == 0 && bMiddle==0 && bBottom==0){
    Serial.println("Correct Posture");
    digitalWrite(buzzer, LOW);
       digitalWrite(motor, LOW);
  }
    if((bTop==0 && bMiddle==0) || (bTop==0 && bBottom==0) || (bMiddle==0 && bBottom==0)){
      if(timerStarted){
        startTimer = millis();
        timerStarted = false;
        Serial.println("timer started");
        digitalWrite(buzzer, LOW);
       digitalWrite(motor, LOW);
       sound = true;
       check = true;
      }
    }
  if(timerStarted == false){
    if (check == true)
      {
        currentMillis = millis();
        check = false;
      }
    if((bTop==1 && bMiddle==1) || (bTop==1 && bBottom==1) || (bMiddle==1 && bBottom==1)){
       timerStarted = true;
       Serial.println("timer off");
       digitalWrite(buzzer, LOW);
       digitalWrite(motor, LOW);
    }
  }
  if(millis()-startTimer > 10000 && timerStarted == false){
    check = true;
    Serial.println("Bad Posture");
    if (sound == true)
      {
        if ((currentMillis - previousMillis1 < buzzerInterval) && (i < buzzerCount) && (buzzerHigh == true))
        {
          digitalWrite(buzzer, HIGH);
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
  }
  
}
