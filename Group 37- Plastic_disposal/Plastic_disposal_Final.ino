#include <Adafruit_NeoPixel.h>
#define PIN 6 
#define NUMPIXELS 8 

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 10 // Time (in milliseconds) to pause between pixels
int led=13;
const int trigPin = 8;
const int echoPin = 9;
long duration;
int distance;

void setup() {
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
pinMode(led,OUTPUT);
pixels.begin();
Serial.begin(9600);
}

void loop() {
pixels.clear();
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance= duration*0.034/2;
Serial.print("Distance: ");
Serial.println(distance);


if (distance <= 15){
digitalWrite(led, HIGH);
pixels.setPixelColor(0,150,0,0);
pixels.setPixelColor(1,0,150,0);
pixels.setPixelColor(2,0,0,150);
pixels.setPixelColor(3,150,0,0);
pixels.setPixelColor(4,0,150,0);
pixels.setPixelColor(5,0,0,150);
pixels.setPixelColor(6,150,0,150);
pixels.setPixelColor(7,150,150,150);
pixels.show();
delay(250);
pixels.setPixelColor(0,250,0,0);
pixels.setPixelColor(1,0,250,0);
pixels.setPixelColor(2,0,0,250);
pixels.setPixelColor(3,250,0,0);
pixels.setPixelColor(4,0,250,0);
pixels.setPixelColor(5,0,0,250);
pixels.setPixelColor(6,250,0,250);
pixels.setPixelColor(7,250,250,250);
pixels.show();
delay(500);
pixels.setPixelColor(0,250,250,0);
pixels.setPixelColor(1,0,250,250);
pixels.setPixelColor(2,250,0,250);
pixels.setPixelColor(3,250,250,0);
pixels.setPixelColor(4,0,250,250);
pixels.setPixelColor(5,250,0,250);
pixels.setPixelColor(6,250,250,250);
pixels.setPixelColor(7,250,250,0);
pixels.show();
delay(500);
//for(int g =0;g<=7;g++){
//  for(int i = 150; i<256; i=i+50){
//    for(int q = 150; q<256; q=q+50){
//      for(int w = 150; w<256; w=w+50){
//        pixels.setPixelColor(g,i,q,w);
//        delay(250);
//        pixels.show();
//      }
//      pixels.show();
//    }
//    pixels.show();
//  }
//  pixels.show();
//}
}

else{
digitalWrite(led, LOW);//if threshold not reached, LED remains off
for(int y=0;y<=7;y++){
pixels.setPixelColor(y,0,0,0);
pixels.show();   // Send the updated pixel colors to the hardware.
delay(DELAYVAL);
}
}}
      
