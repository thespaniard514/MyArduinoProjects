const int lightSensorPin = A0; // light sensor analog
// LED pins (green for closed/safe red for open/danger)
const int redLedPin = 4;
const int greenLedPin = 5;
// I am using 2 relays to control the motor direction instead of a proper motor controller (easier to get my hands on)
// therefore I am using  the two pins below
const int relayTwoPin = 6; 
const int relayOnePin = 7;
// the reed switches sit at the top and bottom of the door with a magnet in the door to trigger the sensors
const int bottomReedPin = 11;
const int topReedPin = 12;
// timeout for the loop (in case something goes wrong)
const int timeout  = 10000;

bool doorOpen = false; // start off with door closed status
int analogValue = 0; // init the analog value to zero
int numTrys = 0;
bool fatalError = false; // used to trigger an error if loop happens too long

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(relayOnePin, OUTPUT);
  pinMode(relayTwoPin, OUTPUT);
  pinMode(bottomReedPin, INPUT);
  pinMode(topReedPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // if fatal error is set, blink the leds
  // and don't let the code run
  if(fatalError){
    digitalWrite(ledPin, LOW);
    delay(500);
    digitalWrite(ledPin, HIGH);
    delay(500);
  }else{
    analogValue = analogRead(lightSensorPin);
    Serial.print("Light Sensor Reading ");
    Serial.println(analogValue);
    Serial.print("Bottom Reed Sensor: ");
    Serial.println(digitalRead(bottomReedPin));
    Serial.print("Top Reed Sensor: ");
    Serial.println(digitalRead(topReedPin));

    
    if(analogValue < 5){
      Serial.print("Door open status: ");
      Serial.println(doorOpen);
      if(digitalRead(ledPin) == LOW){
        digitalWrite(ledPin, HIGH);
      }
      if(doorOpen){
        close_door();
      }
    }else if(analogValue > 15){
      Serial.print("door open status: ");
      Serial.println(doorOpen);
      if(digitalRead(ledPin) == HIGH){
        digitalWrite(ledPin, LOW);
      }
      if(!doorOpen){
        open_door();
      }
    }
    delay(10000);
  }
}

void close_door(){
  Serial.println("Closing Door");
  digitalWrite(relayTwoPin, HIGH);
  numTrys = 0;  
  while(digitalRead(bottomReedPin) == LOW){
//    if(numTrys >= timeout){
//      fatalError = true;
//      break;
//    }
//    numTrys += 200;
    delay(200);
  }
  delay(400);
  digitalWrite(relayTwoPin, LOW);
  doorOpen = false;
  Serial.println("Door is Closed");
}

void open_door(){
  Serial.println("Opening Door");
  digitalWrite(relayOnePin, HIGH);
  
  numTrys = 0;
  while(digitalRead(topReedPin) == LOW){
//    if(numTrys >= timeout){
//      fatalError = true;
//      break;
//    }
//    numTrys += 1;
  }
  digitalWrite(relayOnePin, LOW);
  doorOpen = true;
  Serial.println("Door is Open");
}

