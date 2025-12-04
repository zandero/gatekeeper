#include <Arduino.h>
#include <AccelStepper.h>
#include "BlastGate.h"

#define MOVE_FACTOR 3

BlastGate::BlastGate(uint8_t _motorEnablePin, uint8_t _motorStepPin, uint8_t _motorDirectionPin, long _gateOpenPosition, uint8_t _gateClosedSwitchPin, bool _invertDirection) {
  gateClosedSwitchPin = _gateClosedSwitchPin;
  gateOpenPosition = _gateOpenPosition;

  // invert direction (yes / no)
  invertDirection = _invertDirection;

  // create motor instance
  motor = AccelStepper(1, _motorStepPin, _motorDirectionPin); 

  pinMode(_motorEnablePin, OUTPUT);
  digitalWrite(_motorEnablePin, LOW);
  motor.setMaxSpeed(MAX_MOTOR_SPEED);

  // initialize switch
  pinMode(gateClosedSwitchPin, INPUT_PULLUP);
}

void BlastGate::close() {
  if (!isClosed()) { 
    motor.setSpeed(MAX_MOTOR_SPEED);

    if (invertDirection) {
      motor.moveTo(MOVE_FACTOR * gateOpenPosition); 
    }
    else {
      motor.moveTo(-MOVE_FACTOR * gateOpenPosition); 
    }
  
    motor.runSpeedToPosition();
  }
  else {
    motor.setCurrentPosition(0);  // reset position to zero once closed
  }
}

void BlastGate::open() {
  if (!isOpen()) {
    motor.setSpeed(MAX_MOTOR_SPEED);
    if (invertDirection) {
      motor.moveTo(-MOVE_FACTOR * gateOpenPosition); 
    }
    else {
      motor.moveTo(MOVE_FACTOR * gateOpenPosition); 
    }
    motor.runSpeedToPosition();
  }
}

bool BlastGate::isClosed() {
  return digitalRead(gateClosedSwitchPin) == LOW;
}

bool BlastGate::isOpen() {
  if (invertDirection) {
    return motor.currentPosition() == -gateOpenPosition;
  }
  return motor.currentPosition() == gateOpenPosition;
}

bool BlastGate::isMoving() {
  if (invertDirection) {
    return motor.currentPosition() < 0 && motor.currentPosition() > -gateOpenPosition;
  }
  return motor.currentPosition() > 0 && motor.currentPosition() < gateOpenPosition;
}

