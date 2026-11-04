#include <Arduino.h>
#include <BlastGate.h>

// Debug on/off of serial messages is set in platformio.ini (build_flags = -D DEBUG=1 / -D DEBUG=0)
// so it applies to main.cpp and the libraries
#ifndef DEBUG
  #define DEBUG 0
#endif
#if DEBUG
  #define debug(x) Serial.print(x)
  #define debugln(x) Serial.println(x)
#else
  #define debug(x)
  #define debugln(x)
#endif


// Gate switches
#define SWITCH_GATE_A 14   // D14 green

// Gate motors
#define MOTOR_A_STEP_PIN 54     // X - axis
#define MOTOR_A_DIR_PIN 55
#define MOTOR_A_ENABLE_PIN 38

// motor speed and max open position
#define MAX_MOTOR_SPEED 800L
#define OPEN_ROTATION 1425L     // (recommended for DX100G Blast Gate 100mm with DRV8825 stepper driver)

// invert motor spin direction (same setting as the main GateKeeper firmware)
#define INVERT_MOTOR_DIRECTION true

// pause once the gate is fully open / closed before moving it the other way
#define PAUSE_MS 2000
// gate not moving for this long while closing -> the closed switch was never pressed
#define STALL_MS 1000

// Associate motor and gate switches
BlastGate gateA(MOTOR_A_ENABLE_PIN, MOTOR_A_STEP_PIN, MOTOR_A_DIR_PIN, OPEN_ROTATION, SWITCH_GATE_A, INVERT_MOTOR_DIRECTION);

// start by closing, so the closed switch sets position 0 even if the gate was left partly open
bool opening = false;
unsigned long cycle = 0;

// stall detection while closing
long lastPosition = 0;
unsigned long lastMoveMs = 0;
bool stallReported = false;

void setup() {
  Serial.begin(9600);
  debugln("Starting up BlastGate test ... ");
  debugln("Closing gate to find closed position (switch) ...");
}

// called once the gate reached the end of a move: log, pause and reverse direction
void reverse(const char* state) {
  debug("Cycle ");
  debug(cycle);
  debug(": gate is ");
  debug(state);
  debug(" pos=");
  debugln(gateA.getPosition());

  delay(PAUSE_MS);

  opening = !opening;
  lastMoveMs = millis();
  stallReported = false;
}

void loop() {
  // NOTE: no serial output while the gate is moving, printing at 9600 baud slows down the motor steps

  if (opening) {
    gateA.open();

    if (gateA.isOpen()) {
      cycle++;
      reverse("OPEN");
    }
  }
  else {
    gateA.close();

    if (gateA.isClosed()) {
      reverse("CLOSED");
      return;
    }

    // the motor stops after a limited distance, if the switch is never pressed the gate would wait here forever
    long position = gateA.getPosition();
    if (position != lastPosition) {
      lastPosition = position;
      lastMoveMs = millis();
    }
    else if (!stallReported && millis() - lastMoveMs > STALL_MS) {
      debugln("Gate stopped but closed switch is not pressed - check the switch wiring / position!");
      stallReported = true;
    }
  }
}

