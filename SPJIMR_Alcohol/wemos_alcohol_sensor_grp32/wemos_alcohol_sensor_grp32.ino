#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        4 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 2 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

const int AOUTpin=A0;//the AOUT pin of the alcohol sensor goes into analog pin A0 of the arduino
const int DOUTpin=5;//the DOUT pin of the alcohol sensor goes into digital pin D8 of the arduino
const int ledPin=2;//the anode of the LED connects to digital pin D13 of the arduino
const int relay=12;
int limit;
int value;
#include <ESP8266WiFi.h>

const char* ssid = "Aashit";
const char* password = "987654321";
const char* host = "maker.ifttt.com";
const char* apiKey = "dsjPrOPpc5XLamAbFKKSo0";
//const char alcohol_sensed;

void setup() {

    pixels.begin();
    Serial.begin(115200);
    delay(100);
    //Serial.println("Preparing the Door Status Monitor project...");
    pinMode(DOUTpin, INPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(relay, OUTPUT);

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
}

void loop() {
pixels.clear();
value= analogRead(AOUTpin);//reads the analaog value from the alcohol sensor's AOUT pin
limit= 900;//reads the digital value from the alcohol sensor's DOUT pin
Serial.print("Alcohol value: ");
Serial.println(value);//prints the alcohol value
Serial.print("Limit: ");
Serial.println(limit);//prints the limit reached as either LOW or HIGH (above or underneath)
delay(100);

if (value < limit){
digitalWrite(ledPin, LOW);//if threshold not reached, LED remains off
digitalWrite(relay, LOW);
pixels.setPixelColor(0,0,0,0);
pixels.show();   

   // Send the updated pixel colors to the hardware.

//delay(DELAYVAL); // Pause before next pass through loop
}
else if (value>=limit){
digitalWrite(ledPin, HIGH);//if limit has been reached, LED turns on as status indicator
digitalWrite(relay, HIGH);
connection();
 for(int x=0;x<50;x++){ 
  pixels.clear();
  pixels.setPixelColor(0,0,0,250);
  pixels.setPixelColor(1,0,0,250);
  pixels.show();
  delay(250);
  pixels.clear();
  pixels.setPixelColor(0,250,0,0);
  pixels.setPixelColor(1,250,0,0);
  pixels.show();
  delay(250);
  pixels.clear();
 }

//delay(60000);
}
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
    
          String url = "/trigger/alcohol_sensed/with/key/";
          url += apiKey;
          
          Serial.print("Requesting URL: ");
          Serial.println(url);
          client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                       "Host: " + host + "\r\n" + 
                       "Content-Type: application/x-www-form-urlencoded\r\n" + 
                       "Content-Length: 13\r\n\r\n" +
                       "value1=" + "alcohol_sensed" + "\r\n");
      }
