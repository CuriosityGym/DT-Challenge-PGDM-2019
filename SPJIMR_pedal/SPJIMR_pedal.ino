int switchPin = 0;              // switch is connected to pin 2
int val;                        // variable for reading the pin status
int buttonState;                // variable to hold the button state
int buttonPresses = 0;          // how many times the button has been pressed
unsigned long startTime;
unsigned long endTime;
unsigned long duration;
byte timerRunning;
void(* resetFunc) (void) = 0;

void setup() {
  pinMode(switchPin, INPUT);    // Set the switch pin as input
  Serial.begin(9600);           // Set up serial communication at 9600bps
  buttonState = digitalRead(switchPin);   // read the initial state
}

void loop(){
  val = digitalRead(switchPin);      // read input value and store it in val
  if (val != buttonState) {          // the button state has changed!
    if (val == LOW) {                // check if the button is pressed
      if (timerRunning == 0 && digitalRead(switchPin) == LOW){ // button pressed & timer not running already
  startTime = millis();
  timerRunning = 1;
  }
    if (timerRunning == 1 && digitalRead(switchPin) == HIGH){ // timer running, button released
  endTime = millis();
  timerRunning = 0;
  duration = endTime - startTime;
  if (duration<30000){
    resetFunc();
  }
  //Serial.print ("button press time in milliseconds: ");
  //Serial.println (duration);
  }
      buttonPresses++;               // increment the buttonPresses variable
      Serial.print("Button has been pressed ");
      Serial.print(buttonPresses);
      Serial.println(" times");
      
    }
  }
  buttonState = val;                 // save the new state in our variable
}
