const int lightSensorPin = A0; // light sensor analog

// I am using 2 relays to control the motor direction instead of a proper motor controller (easier to get my hands on)
// therefore I am using  the two pins below
const int counterClockwiseRelayPin = 4; 
const int clockwiseRelayPin = 5;
// the reed switches sit at the top and bottom of the door with a magnet in the door to trigger the sensors
const int bottomReedPin = 7;
const int topReedPin = 8;
const int waitTimeLightSensor = 20000;

bool doSerial = true;
int analogValue = 0; // init the analog value to zero

void motorOff(){
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
  if(doSerial){
    Serial.begin(9600);  
  }  
}

void loop() {
  analogValue = analogRead(lightSensorPin);
  if(doSerial){
    Serial.print("Light Sensor Reading ");
    Serial.println(analogValue);
    Serial.print("Bottom Reed Sensor: ");
    Serial.println(digitalRead(bottomReedPin));
    Serial.print("Top Reed Sensor: ");
    Serial.println(digitalRead(topReedPin));
  }
  
  if(analogValue < 7){
    close_door();
  }else if(analogValue > 18){
    open_door();
  }
  delay(waitTimeLightSensor);
}



