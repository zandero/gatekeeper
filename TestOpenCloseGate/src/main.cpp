#include <Arduino.h>
#include <BlastGate.h>

// Added for easier debug on/off of serial messages
// (set DEBUG=false) once everything is running as expected (but can be left on DEBUG=true - does not affect the functionality)
#define DEBUG true
#ifdef DEBUG
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

// Associate motor and gate switches
BlastGate gateA(MOTOR_A_ENABLE_PIN, MOTOR_A_STEP_PIN, MOTOR_A_DIR_PIN, OPEN_ROTATION, SWITCH_GATE_A);

void setup() {
  if (DEBUG) {
    Serial.begin(9600);
    debugln("Starting up BlastGate test ... ");
  }

  debugln("Initialization finished");
}

void loop() {

  if (gateA.isMoving()) {
    debugln("Gate is moving ... ");
  }

  if (gateA.isClosed()) {
    debugln("Gate is closed");
  }

  if (gateA.isOpen()) {
    debugln("Gate is open");
  }

  gateA.open();
}

