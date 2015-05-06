#include "/Users/BlaineKillen/Documents/Arduino/libraries/Arduino-IRremote-master/IRremote.h"
#include <IRremoteInt.h>


// IR receiver type is TSOP382 when referencing the datasheet
const int RECV_PIN = 3;
IRrecv irrecv(RECV_PIN);
decode_results results;

// require pwm, pins 11,10,9 and 6 are PWM capable
const int positivePin = 11;
const int negativePin = 10;

int currentSpeed;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  
  // set the pin mode of all of MOSFETs to output pins
  pinMode(positivePin, OUTPUT);
  pinMode(negativePin, OUTPUT);
  
  // initialize the motor to the hold position, awaiting user input
  holdPosition();
}

void loop() {
  if (irrecv.decode(&results)) {
    int value = results.value;
    //Serial.println(value, HEX);
    
    if( value == 0x6621 || value == 0x2421) {
      stepUpVoltage();
    } else if( value == 0x2621 || value == 0x6421) {
      stepDownVoltage();
    } else if( value == 0x5621) {
      maxDown();
    } else if( value == 0x1621) {
      maxUp();
    } else if( value == 0x1D21) {
      holdPosition();
    }
    
    irrecv.resume(); // Receive the next value
  }
}

void stepUpVoltage() {
  Serial.println("step up");
  
  if(currentSpeed == -105) {
    currentSpeed = 105;
  } else if (currentSpeed != 255) {
    currentSpeed += 10;
  }
  
  changeVoltage();
}

void stepDownVoltage() {
  Serial.println("step down");
  
  if(currentSpeed == 105) {
    currentSpeed = -105;
  } else if (currentSpeed != -255) {
    currentSpeed -= 10;
  }
  
  changeVoltage();
}

void holdPosition() {
  Serial.println("stop");
  
  // this voltage opens barely opens the positive gate so the motor can 
  // remain in place in space
  currentSpeed = 100;
  changeVoltage();
}

void maxUp() {
  Serial.println("max up");
  
  currentSpeed = 255;
  changeVoltage();
}

void maxDown() {
  Serial.println("max down");
  
  currentSpeed = -255;
  changeVoltage();
}

void changeVoltage() {
  if(currentSpeed > 0) {
    analogWrite(positivePin, currentSpeed);
  } else if(currentSpeed < 0) {
    analogWrite(negativePin, (currentSpeed * -1));
  } else {
    analogWrite(positivePin, 0);
    analogWrite(negativePin, 0);
  }
}
