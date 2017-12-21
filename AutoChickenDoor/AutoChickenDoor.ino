const int lightSensorPin = A0; // light sensor analog
// LED pins (green for closed/safe red for open/danger)
const int redLedPin = 2;
const int greenLedPin = 3;
// I am using 2 relays to control the motor direction instead of a proper motor controller (easier to get my hands on)
// therefore I am using  the two pins below
const int counterClockwiseRelayPin = 4; 
const int clockwiseRelayPin = 5;
// the reed switches sit at the top and bottom of the door with a magnet in the door to trigger the sensors
const int bottomReedPin = 11;
const int topReedPin = 12;
const int waitTimeLightSensor = 30000;

bool doSerial = false;
int analogValue = 0; // init the analog value to zero

void close_door(){
  // loop until the reed sensor shows HIGH
  while(digitalRead(bottomReedPin) == LOW){
    if(digitalRead(counterClockwiseRelayPin) == LOW){
      if(doSerial){Serial.println("Closing Door");}
      digitalWrite(counterClockwiseRelayPin, HIGH);  
    }    
    delay(200);
  }
  delay(400);
  digitalWrite(counterClockwiseRelayPin, LOW);  
  if(doSerial){Serial.println("Door is Closed");}
}

void open_door(){
  // loop until the reed sensor shows HIGH
  while(digitalRead(topReedPin) == LOW){
    if(digitalRead(clockwiseRelayPin) == LOW){
      if(doSerial){Serial.println("Opening Door");}
      digitalWrite(clockwiseRelayPin, HIGH);
    }
    delay(100);
  }
  digitalWrite(clockwiseRelayPin, LOW);
  if(doSerial){Serial.println("Door is Open");}
}

void setup() {
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(clockwiseRelayPin, OUTPUT);
  pinMode(counterClockwiseRelayPin, OUTPUT);
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
  
  if(analogValue < 2){
    if(digitalRead(greenLedPin) == LOW){
      digitalWrite(greenLedPin, HIGH);
    }
    if(digitalRead(redLedPin) == HIGH){
      digitalWrite(redLedPin, LOW);
    }
    close_door();
  }else if(analogValue > 20){
    if(digitalRead(greenLedPin) == HIGH){
      digitalWrite(greenLedPin, LOW);
    }
    if(digitalRead(redLedPin) == LOW){
      digitalWrite(redLedPin, HIGH);
    }
    open_door();
    }
  delay(waitTimeLightSensor);
}



