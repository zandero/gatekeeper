#ifndef LIGHTBLINK_h
#define LIGHTBLINK_h

#include <Timer.h>

/**
 * Simplifies logic for light / led blink indication
 */
class BlinkLight
{
  protected:
    uint8_t lightPin;          // switch detecting when the gate is closed
    unsigned long blinkTimeMs; // time in ms between blinks
    Timer blinkCheck;          // timer for blinking light
    bool blinking;             // true - light is blinking, false - light is on or off
    int8_t timerId;
    
  public:
    /**
     *
     * @param _lightPin         light PIN
     * @param _blinkIntervalMs  interval between blinks in ms
     */
    BlinkLight(uint8_t _ligthPin, unsigned long _blinkIntervalMs);

    /**
     * Call in loop() to check and set state of light if needed
     */
    void update();

    /**
     * Should be called whenever we want the light to be blinking 
     */
    void blink();

    /**
     * Turns light on
     */
    void on();

    /**
     * Turns light off
     */
    void off();
};

#endif