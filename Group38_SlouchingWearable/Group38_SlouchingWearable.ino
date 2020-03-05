#include <Wire.h>

#define SLOUCH_ANGLE        15.0      // allowable slouch angle (deg)
#define SLOUCH_TIME         7000      // allowable slouch time (secs) 
//#define GRAVITY             9.80665   // standard gravity (m/s^s)
//#define RAD2DEG             52.29578  // convert radians to degrees
 

//variables to store values from mpu6050

long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;

long gyroX, gyroY, gyroZ;
float rotX, rotY, rotZ;
float gyroX_cal, gyroY_cal, gyroZ_cal;
float angle_pitch, angle_roll;
float angle_roll_acc, angle_pitch_acc;
float angle_pitch_output, angle_roll_output;
int acc_calibration_value = 1000;                            //Enter the accelerometer calibration value
float angle_acc;

long loop_timer;
int servoXpos=90;
int servoYpos=90;
int count = 0;
int button = 5;
int led = 1;

//slouch detector variables
float currentAngle;
float targetAngle;
unsigned long slouchStartTime;
bool slouching;
//byte motor = 1;
byte buzzer = 3;

void setup(){
  Serial.begin(9600);

 
  Wire.begin();
  setupMPU();       //set mpu6050 registers
  //delay(1000);
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  //pinMode(motor, OUTPUT);
  digitalWrite(led, HIGH);
  pinMode(button, INPUT);
  
  Serial.println("caliberating MPU6050");  
  for(int i=0; i<2000; i++){
    if(i %125 == 0){Serial.print("."); }
    recordGyroRegisters();
    gyroX_cal += gyroX;
    gyroY_cal += gyroY;
    gyroZ_cal += gyroZ;
    delayMicroseconds(3700);
  }
  gyroX_cal /= 2000;
  gyroY_cal /= 2000;
  gyroZ_cal /= 2000;
  Serial.print("gyroX_cal: ");
  Serial.print(gyroX_cal);
  Serial.print("  gyroY_cal: ");
  Serial.print(gyroY_cal);
  Serial.print("  gyroZ_cal: ");
  Serial.print(gyroZ_cal);
  delay(500);
  digitalWrite(led,LOW);
  
  // Initialize target angle to zero.
  targetAngle = 0;
  //loop_timer = micros();
}

void loop(){
  recordAccelRegisters();
  recordGyroRegisters();
  
  gyroX -= gyroX_cal;
  gyroY -= gyroY_cal;
  gyroZ -= gyroZ_cal;
  //printData();
  angle_pitch += gyroX * 0.000122;
  angle_roll += gyroY * 0.000122;

   //0.000001066 = 0.0000611 * (3.142(PI) / 180degr) The Arduino sin function is in radians
  angle_pitch += angle_roll * sin(gyroZ * 0.000002131);               //If the IMU has yawed transfer the roll angle to the pitch angel
  angle_roll -= angle_pitch * sin(gyroZ * 0.000002131);               //If the IMU has yawed transfer the pitch angle to the roll angel
  
  currentAngle = angle_pitch;
  Serial.println(currentAngle);
  if(digitalRead(button) == LOW){
    targetAngle = currentAngle;
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);  
  }
  //angle_pitch = constrain(angle_pitch, -90.00, 90.00);
  //angle_roll = constrain(angle_roll, -90.00, 90.00);
  // = map(angle_roll, 90.00,-90.00,0,180);
  //servoYpos = map(angle_pitch, -90.00,90.00,0,180);
   // Check for slouching
  if (currentAngle - targetAngle > SLOUCH_ANGLE) {
    if (!slouching) slouchStartTime = millis();
    slouching = true;
  } else {
    slouching = false;
  }
 
  // If we are slouching
  if (slouching) {
    // Check how long we've been slouching
    if (millis() - slouchStartTime > SLOUCH_TIME) {
      // Play a tone
      digitalWrite(buzzer, HIGH);
      digitalWrite(led, HIGH);  
    }    
  }
  if(!slouching){
    digitalWrite(buzzer, LOW);
    digitalWrite(led, LOW);
  }
   //count++;
 /*  while(micros() - loop_timer < 8000);{
    if(count==1){
      if(servoXpos >=0 && servoXpos <=180){
         myservoX.write(servoXpos );
        }
     }
    if(count==2){
      count=0;
      if(servoYpos >=0 && servoYpos <=180){
         myservoY.write(servoYpos);
      }
     }
   }
  loop_timer += 8000;*/
  }
  
void setupMPU(){
  Wire.beginTransmission(0b1101000);
  Wire.write(0x6B);
  Wire.write(0b00000000);
  Wire.endTransmission();
  Wire.beginTransmission(0b1101000);
  Wire.write(0x1B);
  Wire.write(0x08);
  Wire.endTransmission();
  Wire.beginTransmission(0b1101000);
  Wire.write(0x1C);
  Wire.write(0x10);
  Wire.endTransmission();
 /* Wire.beginTransmission(0b1101000);
  Wire.write(0x1A);
  Wire.write(0x03);
  Wire.endTransmission(); */
}

//function to read accelerometer values from mpu6050
void recordAccelRegisters(){
  Wire.beginTransmission(0b1101000);  
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6);
  while(Wire.available() < 6);
  accelX = Wire.read() << 8 | Wire.read();
  accelY = Wire.read() << 8 | Wire.read();
  accelZ = Wire.read() << 8 | Wire.read();
}

//function to read gyro values from mpu6050
void recordGyroRegisters(){
  Wire.beginTransmission(0b1101000);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6);
  gyroX = Wire.read() << 8 | Wire.read();
  gyroY = Wire.read() << 8 | Wire.read();
  gyroZ = Wire.read() << 8 | Wire.read();
}
