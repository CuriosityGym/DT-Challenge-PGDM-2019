/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogReadSerial
*/

int sensor1 = A4;
int sensor2 = A5;
int buzzer = A1;
bool carStarted = false;
bool level1 = false;
bool level2 = false;
bool level3 = false;
int count = 0;
// the setup routine runs once when you press reset:
void setup() {
  pinMode(buzzer, OUTPUT);
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  delay(2000);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue1 = analogRead(sensor1);
  int sensorValue2 = analogRead(sensor2);
  
  // print out the value you read:
  Serial.print("sensor value1: ");
  Serial.println(sensorValue1);
  Serial.print("sensor value2: ");
  Serial.println(sensorValue1);
  delay(500);        // delay in between reads for stability
 if(sensorValue1 >15 && sensorValue2 >15 && carStarted == false){
  carStarted = true;
  //level1 = true;
 
  Serial.print("Car Started");
  delay(1500);
 }

if(carStarted){
  if(sensorValue1 <10 && sensorValue2 <10) {
    count++;
     Serial.print("count");
     Serial.println(count);
  }
  if(sensorValue1 >10 && sensorValue2 >10 && count >= 1) {
    count--;
     Serial.print("count");
     Serial.println(count);
  }
  
  delay(1000);
}
 if(count > 5){
  if(sensorValue1 <10 && sensorValue2 <10) {
    for(int i=0; i<10; i++){
      digitalWrite(buzzer, HIGH);
      delay(1000);
      digitalWrite(buzzer, LOW);
      delay(500);
    }
    carStarted = false;
    count =0;
  }
 }
}
