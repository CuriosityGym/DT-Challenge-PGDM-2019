#define BLYNK_PRINT Serial
#include <SPI.h>
/* Set this to a bigger number, to enable sending longer messages */
//#define BLYNK_MAX_SENDBYTES 256

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
//char auth[] = "b94e9a3d6ff142bc8e03c4850218a2b6";

const char* ssid = "ayush";
const char* password = "ayush1234";
const char* host = "maker.ifttt.com";
const char* apiKey = "cqbH8--ELpnb9fldBoszZa";

int hallSensorPin = 13;     
int ledPin =  14;    
int state = 0;
void hallsensor(){
  state = digitalRead(hallSensorPin);
   if (state == LOW) {        
    //digitalWrite(ledPin, HIGH);  
    emailOnButtonPress();
  } 
  else if (state == HIGH) {
    Serial.println("Helmet Worn!");
    //Blynk.email("anuj@curiositygym.com", "Subject: Helmet Worn", "Helmet has been worn!");
  }
}
void emailOnButtonPress()
{
  // *** WARNING: You are limited to send ONLY ONE E-MAIL PER 15 SECONDS! ***

  // Let's send an e-mail when you press the button
  // connected to digital pin 2 on your Arduino

  int isButtonPressed = digitalRead(0); // Invert state, since button is "Active LOW"

  if (isButtonPressed==LOW) // You can write any condition to trigger e-mail sending
  {
    Serial.println("Helmet Not Worn!"); // This can be seen in the Serial Monitor
    //Blynk.email("anuj@curiositygym.com", "Subject: Helmet is NOT Worn", "You have not worn your helmet.");
    connection();

    // Or, if you want to use the email specified in the App (like for App Export):
    //Blynk.email("Subject: Button Logger", "You just pushed the button...");
  }
}

void setup()
{
  //pinMode(ledPin, OUTPUT);      
  pinMode(hallSensorPin, INPUT);
  // Debug console
  Serial.begin(9600);

  //Blynk.begin(auth, ssid, pass);
  // You can also specify sderver:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Send e-mail when your hardware gets connected to Blynk Server
  // Just put the recepient's "e-mail address", "Subject" and the "message body"
  //Blynk.email("anuj@curiositygym.com", "Subject", "My Blynk project is online.");

  // Setting the button
  pinMode(0, INPUT);
  // Attach pin 2 interrupt to our handler
 attachInterrupt(digitalPinToInterrupt(0), hallsensor, CHANGE);

  //int index = 1;
  //float lat = 19.106579;
  //float lon = 72.8413;
  //myMap.location(index, lat, lon, "value");
}

void loop()
{
  //Blynk.run();
  hallsensor();
}

void connection(void){
        Serial.print("connecting to ");
          Serial.println(host);
          
          WiFiClient client;
          const int httpPort = 80;
          if (!client.connect(host, httpPort)) {
            Serial.println("connection failed");
            return;
          }
    
          String url = "/trigger/wear_helmet/with/key/";
          url += apiKey;
          
          Serial.print("Requesting URL: ");
          Serial.println(url);
          client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                       "Host: " + host + "\r\n" + 
                       "Content-Type: application/x-www-form-urlencoded\r\n" + 
                       "Content-Length: 13\r\n\r\n" +
                       "value1=" + "wear_helmet" + "\r\n");
      }
