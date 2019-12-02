/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/
#include "Arduino.h"
 
#include "SoftwareSerial.h"
 
#include "DFRobotDFPlayerMini.h"
#include <Servo.h>
 
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
 
DFRobotDFPlayerMini myDFPlayer;


Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position


// defines pins numbers
const int trigPin = 7;
const int echoPin = 8;
// defines variables
long duration;
int distance;
int trashDistance = 0;
int thresholdDistance = 50;
void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo.write(0);              // tell servo to go to position in variable 'pos'
 // delay(10000);
 Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
 
    if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
 
       Serial.println(F("Unable to begin:"));
 
       Serial.println(F("1.Please recheck the connection!"));
 
       Serial.println(F("2.Please insert the SD card!"));
 
       while(true);
 
    }
 
    Serial.println(F("DFPlayer Mini online."));
 
    myDFPlayer.volume(20);  //Set volume value. From 0 to 30
    delay(2000);
    
}

int measure_distance(){
      // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance= duration*0.034/2;
    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);
    Serial.println(F("DFRobot DFPlayer Mini Demo"));
    return distance; 
    
}


void loop() {
    for(int i=0; i<180; i++){
      trashDistance = measure_distance();
      Serial.println(i);
      myservo.write(i);
      delay(50);
      if(trashDistance <= thresholdDistance){
        Serial.println("trash detected");
        myDFPlayer.play(1);  //Play the first mp3
        delay(5000);
      }
    }
   
}
