const int lightSensorPin = A0; // light sensor analog

const int manualModeBtnPin = 2;
const int toggleUpDownPin = 3;
// I am using 2 relays to control the motor direction instead of a 
// motor controller (easier to get my hands on)
// therefore I am using  the two pins below
const int counterClockwiseRelayPin = 4; 
const int clockwiseRelayPin = 5;
const int ledPin = 6;
// the reed switches sit at the top and bottom of the 
// door with a magnet in the door to trigger the sensors
const int bottomReedPin = 7;
const int topReedPin = 8;
const int downThreshold = 2;
const int upThreshold = 15;
const int waitTimeLightSensor = 30000;
const int waitTimeManualMode = 3000;

bool doSerial = true;
int analogValue = 0; // init the analog value to zero

int buttonState = LOW;   
int manualMode = LOW;    
int toggleState = LOW;

void readManualModeState(){
  int buttonState = digitalRead(manualModeBtnPin);
  if(buttonState == HIGH  && manualMode == 1){    
    if(doSerial){Serial.println("Coming out of manual mode");}
    manualMode = LOW;
    digitalWrite(ledPin, LOW);
    buttonState = LOW;
    delay(2000);
  }else if(buttonState == HIGH  && manualMode == 0){
    if(doSerial){Serial.println("Going into manual mode");}
    manualMode = HIGH;
    digitalWrite(ledPin, HIGH);
    buttonState = LOW;
    delay(2000);
  }
}

void motorOff(){
  // double high on the relays means off
  digitalWrite(counterClockwiseRelayPin, HIGH);  
  digitalWrite(clockwiseRelayPin, HIGH);
}

void close_door(){
  // loop until the reed sensor shows HIGH
  while(digitalRead(bottomReedPin) == LOW){
    if(digitalRead(counterClockwiseRelayPin) == HIGH){
      if(doSerial){Serial.println("Closing Door");}      
      if(digitalRead(clockwiseRelayPin) == LOW){
        digitalWrite(clockwiseRelayPin, HIGH);
      }
      if(digitalRead(counterClockwiseRelayPin) == HIGH){
        digitalWrite(counterClockwiseRelayPin, LOW);
      }
    }
    delay(200);
  }
  delay(400);
  motorOff();
  if(doSerial){Serial.println("Door is Closed");}
}

void open_door(){
  // loop until the reed sensor shows HIGH
  while(digitalRead(topReedPin) == LOW){
    if(digitalRead(clockwiseRelayPin) == HIGH){
      if(doSerial){Serial.println("Opening Door");}
      if(digitalRead(counterClockwiseRelayPin) == LOW){
        digitalWrite(counterClockwiseRelayPin, HIGH);
      }
      if(digitalRead(clockwiseRelayPin) == HIGH){
        digitalWrite(clockwiseRelayPin, LOW);
      }
    }
    delay(200);
  }
  motorOff();
  if(doSerial){Serial.println("Door is Open");}
}

void setup() {
  pinMode(clockwiseRelayPin, OUTPUT);
  digitalWrite(clockwiseRelayPin, HIGH);
  pinMode(counterClockwiseRelayPin, OUTPUT);
  digitalWrite(counterClockwiseRelayPin, HIGH);
  pinMode(bottomReedPin, INPUT);
  pinMode(topReedPin, INPUT);
  pinMode(manualModeBtnPin, INPUT);
  pinMode(toggleUpDownPin, INPUT);
  pinMode(ledPin, OUTPUT);
  if(doSerial){
    Serial.begin(9600);  
  }  
}

void loop() {
  readManualModeState();
  if(manualMode == HIGH){
    toggleState = digitalRead(toggleUpDownPin);
    if(toggleState == HIGH){
      if(digitalRead(topReedPin) == LOW){
        open_door();
        toggleState = LOW;
      }else if(digitalRead(bottomReedPin) == LOW){
        close_door();
        toggleState = LOW;
      }
    }
    delay(waitTimeManualMode);
  }else{
    analogValue = analogRead(lightSensorPin);
    if(doSerial){
      Serial.print("Light Sensor Reading ");
      Serial.println(analogValue);
      Serial.print("Bottom Reed Sensor: ");
      Serial.println(digitalRead(bottomReedPin));
      Serial.print("Top Reed Sensor: ");
      Serial.println(digitalRead(topReedPin));
    }
    
    if(analogValue < downThreshold){
      close_door();
    }else if(analogValue > upThreshold){
      open_door();
    }
    delay(waitTimeLightSensor);
  }
}



