/*
 Created by Rui Santos
 
 All the resources for this project:
 http://randomnerdtutorials.com/
 
 Based on some ESP8266 code examples 
*/

#include <ESP8266WiFi.h>

const char* ssid = "DT_LAB";
const char* password = "fthu@050318";
const char* host = "maker.ifttt.com";
const char* apiKey = "iYiYhj3KyPFEwyVRuJzEb";

int pin = 4;
volatile int state = false;
volatile int flag = false;
const char* door_state = "......";

unsigned long previousMillis = 0; 
const long interval = 3000;




void setup() {
    Serial.begin(115200);
    delay(100);
    Serial.println("Preparing the Door Status Monitor project...");
   

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());  
     Serial.print("connecting to ");
          Serial.println(host);
          
          WiFiClient client;
          const int httpPort = 80;
          if (!client.connect(host, httpPort)) {
            Serial.println("connection failed");
            return;
          }
    
          String url = "/trigger/sos/with/key/";
          url += apiKey;
          
          Serial.print("Requesting URL: ");
          Serial.println(url);
          client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                       "Host: " + host + "\r\n" + 
                       "Content-Type: application/x-www-form-urlencoded\r\n" + 
                       "Content-Length: 13\r\n\r\n" +
                       "value1=" + door_state + "\r\n");
        
      delay(10);
}

void loop() {


         
}
