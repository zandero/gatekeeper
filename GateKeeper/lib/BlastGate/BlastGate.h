#ifndef BLASTGATE_h
#define BLASTGATE_h

#include <AccelStepper.h>

#define MAX_MOTOR_SPEED 2400L

/**
 * Simplifies logic of stepper motor movement with open/close commands
 */
class BlastGate
{
  protected:
    uint8_t gateClosedSwitchPin;  // switch detecting when the gate is closed
    long gateOpenPosition;        // max position of motor (ie. open)
    bool invertDirection;         // invert direction of motor spin
    AccelStepper motor;

  public:
    /**
     *
     * @param _motorEnablePin stepper motor enable PIN
     * @param _motorStepPin  stepper motor step pin
     * @param _motorDirectionPin stepper motor direction PIN (close direction move is negative, open direction move is positive)
     * @param _gateOpenPosition gate max open position for stepper motor (aka max movement from 0)
     * @param _gateClosedSwitchPin gate close switch PIN, signaling that the gate is closed
     * @param _invertDirection invert direction of motor spinning (true invert), false default
     */
    BlastGate(uint8_t _motorEnablePin, uint8_t _motorStepPin, uint8_t _motorDirectionPin, long _gateOpenPosition, uint8_t _gateClosedSwitchPin, bool _invertDirection);

    /**
     * Triggers the gate to close
     * gate will continue to close until the gate close switch is depressed
     */
    void close();

    /**
     * Triggers the gate to open
     * gate will continue to open once it reaches the configured gateOpenPosition
     */
    void open();

    /**
     * Checks if get is closed
     * @return gate is considered closed when get close switch is pressed
     */
    bool isClosed();

    /**
     * Fully open is when stepper motor position is equal to configured getOpenPosition
     * @return true when gate is considered fully open, false otherwise
     */
    bool isOpen();

    /**
     * Check if open/close operation is in progress
     * @return true if gate is either being opened or closed, false get is open or closed
     */
    bool isMoving();
};

#endif