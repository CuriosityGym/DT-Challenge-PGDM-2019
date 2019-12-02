#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        6 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 16 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

const int AOUTpin=A5;//the AOUT pin of the alcohol sensor goes into analog pin A0 of the arduino
const int DOUTpin=6;//the DOUT pin of the alcohol sensor goes into digital pin D8 of the arduino
const int ledPin=3;//the anode of the LED connects to digital pin D13 of the arduino

int limit;
int value;

void setup() {
Serial.begin(115200);//sets the baud rate
pinMode(DOUTpin, INPUT);//sets the pin as an input to the arduino
pinMode(ledPin, OUTPUT);//sets the pin as an output of the arduino
pixels.begin();
}

void loop()
{
pixels.clear();
value= analogRead(AOUTpin);//reads the analaog value from the alcohol sensor's AOUT pin
limit= 900;//reads the digital value from the alcohol sensor's DOUT pin
Serial.print("Alcohol value: ");
Serial.println(value);//prints the alcohol value
Serial.print("Limit: ");
Serial.print(limit);//prints the limit reached as either LOW or HIGH (above or underneath)
delay(100);
if (value >= limit){
digitalWrite(ledPin, HIGH);//if limit has been reached, LED turns on as status indicator
pixels.setPixelColor(0,0,0,255);
pixels.show();   // Send the updated pixel colors to the hardware.
delay(DELAYVAL); // Pause before next pass through loop
}
else{
digitalWrite(ledPin, LOW);//if threshold not reached, LED remains off
pixels.setPixelColor(0,0,0,0);
pixels.show();   // Send the updated pixel colors to the hardware.
delay(DELAYVAL);
}
}
