int sensorPin = A0; 
int sensorPin1 = A1; 
int sensorValue;  
int sensorValue1; 
int limit = 850; 

void setup() {
 Serial.begin(9600);
 pinMode(13, OUTPUT);
 pinMode(12, OUTPUT);
}

void loop() {

 sensorValue = analogRead(sensorPin);
 sensorValue1 = analogRead(sensorPin1); 
 Serial.println("Analog Value : ");
 Serial.println(sensorValue);
 Serial.println("Analog Value1 : ");
 Serial.println(sensorValue1);
 
 if (sensorValue<limit) {
 digitalWrite(13, HIGH); 
 }
 else {
 digitalWrite(13, LOW); 
 }
 if (sensorValue1<limit) {
 digitalWrite(12, HIGH); 
 }
 else {
 digitalWrite(12, LOW); 
 }
 delay(1000); 
}
