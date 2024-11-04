#include "BlinkLight.h"
#include <Arduino.h>
#include <Timer.h>

BlinkLight::BlinkLight(uint8_t _lightPin, unsigned long _blinkTimeMs) {

    lightPin = _lightPin;
    blinkTimeMs = _blinkTimeMs;
    blinking = false;


    blinkCheck = Timer();

    pinMode(lightPin, OUTPUT);
}

void BlinkLight::blink() {
    if (!blinking) {
        timerId = blinkCheck.oscillate(lightPin, blinkTimeMs, HIGH);
        blinking = true;
    }
}

void BlinkLight::update() {
    blinkCheck.update();
}

void BlinkLight::on() {
    if (blinking) {
        blinkCheck.stop(timerId);
    }

    blinking = false;
    digitalWrite(lightPin, HIGH);
}

void BlinkLight::off() {
    if (blinking) {
        blinkCheck.stop(timerId);
    }

    blinking = false; 
    digitalWrite(lightPin, LOW);
}


