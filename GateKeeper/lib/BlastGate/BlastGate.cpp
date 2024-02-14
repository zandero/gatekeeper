#include <Arduino.h>
#include <AccelStepper.h>
#include "BlastGate.h"

BlastGate::BlastGate(uint8_t _motorEnablePin, uint8_t _motorStepPin, uint8_t _motorDirectionPin, long _gateOpenPosition, uint8_t _gateClosedSwitchPin) {
  gateClosedSwitchPin = _gateClosedSwitchPin;
  gateOpenPosition = _gateOpenPosition;

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
    motor.moveTo(-2 * gateOpenPosition); 
    motor.runSpeedToPosition();
  }
  else {
    motor.setCurrentPosition(0);  // reset position to zero once closed
  }
}

void BlastGate::open() {
  if (!isOpen()) {
    motor.setSpeed(MAX_MOTOR_SPEED);
    motor.moveTo(gateOpenPosition); 
    motor.runSpeedToPosition();
  }
}

bool BlastGate::isClosed() {
  return digitalRead(gateClosedSwitchPin) == LOW;
}

bool BlastGate::isOpen() {
  return motor.currentPosition() == gateOpenPosition;
}

bool BlastGate::isMoving() {
  return motor.currentPosition() > 0 && motor.currentPosition() < gateOpenPosition;
}