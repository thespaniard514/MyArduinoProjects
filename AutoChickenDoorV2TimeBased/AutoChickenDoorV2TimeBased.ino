const int lightSensorPin = A0; // light sensor analog
const int enablePin = 3;
const int goUpPin = 4; 
const int goDownPin = 5;
const int bottomReedPin = 7;
const int topReedPin = 8;

const int downThreshold = 1;
const int upThreshold = 15;
const int waitTimeLightSensor = 15000;
const int waitTimeManualMode = 3000;

bool doSerial = true;
int analogValue = 0; // init the analog value to zero

int buttonState = LOW;
int manualMode = LOW;   
int toggleState = LOW;

unsigned long starttime = 0;
unsigned long endtime = 0;

int closeTime = 9000;
int openTime = 11000;

bool isOpen = true;

void motorOff(){
  digitalWrite(goUpPin, LOW);
  digitalWrite(goDownPin, LOW);
}

void motorOnDown(){
  digitalWrite(goUpPin, HIGH);
  digitalWrite(goDownPin, LOW);
}

void motorOnUp(){
  digitalWrite(goUpPin, LOW);
  digitalWrite(goDownPin, HIGH);
}

void closeDoor(){  
  if(doSerial){Serial.println("Closing Door");} 
  starttime = millis();
  while(digitalRead(bottomReedPin) == LOW && (millis() - starttime) <= closeTime){
     motorOnDown();
  }
  motorOff();
  isOpen = false;
  if(doSerial){Serial.println("Door is Closed");}
}

void openDoor(){
  if(doSerial){Serial.println("Opening Door");}
  starttime = millis();
  while(digitalRead(topReedPin) == LOW && (millis() - starttime) <= openTime){
     motorOnUp();
  }
  motorOff();
  isOpen = true; 
  if(doSerial){Serial.println("Door is Open");}
}

void setup() {  
  pinMode(goUpPin, OUTPUT);
  pinMode(goDownPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(bottomReedPin, INPUT);
  pinMode(topReedPin, INPUT);
  digitalWrite(goUpPin, LOW);
  digitalWrite(goDownPin, LOW);
  analogWrite(enablePin, 255);
  if(doSerial){
    Serial.begin(9600);  
  }  
  if(digitalRead(bottomReedPin) == HIGH){
    isOpen = false;
  }
}

void loop() {
    analogValue = analogRead(lightSensorPin);
    if(doSerial){
      Serial.print("Light Sensor Reading ");
      Serial.println(analogValue);
      Serial.print("Bottom Reed: ");
      Serial.println(digitalRead(bottomReedPin));
      Serial.print("Top Reed: ");
      Serial.println(digitalRead(topReedPin));
    }
    
    if(analogValue < downThreshold && isOpen){
      closeDoor();
    }else if(analogValue > upThreshold && !isOpen){
      openDoor();
    }
    delay(waitTimeLightSensor);
}



