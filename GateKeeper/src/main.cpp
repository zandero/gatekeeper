#include <Arduino.h>
#include "BlastGate.h"
#include "MachineMonitor.h"
#include "BlinkLight.h"

// Debug on/off of serial messages is set in platformio.ini (build_flags = -D DEBUG=1 / -D DEBUG=0)
// so it applies to main.cpp and the libraries (e.g. MachineMonitor consumption logging)
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

// Machine energy monitors
#define JOINTER_PIN 3           // A3 (red)
#define CIRCULAR_SAW_PIN 4      // A4 (yellow)
#define BAND_SAW_PIN 5          // A5 (blue)

// Gate switches
#define SWITCH_GATE_A 14   // D14 green
#define SWITCH_GATE_B 15   // D15 white
#define SWITCH_GATE_C 18   // D18 purple
#define SWITCH_GATE_D 19   // D19 black

// Gate motors
#define MOTOR_A_STEP_PIN 54     // X - axis
#define MOTOR_A_DIR_PIN 55
#define MOTOR_A_ENABLE_PIN 38

#define MOTOR_B_STEP_PIN 60     // Y - axis
#define MOTOR_B_DIR_PIN 61
#define MOTOR_B_ENABLE_PIN 56

#define MOTOR_C_STEP_PIN 46     // Z - axis
#define MOTOR_C_DIR_PIN 48
#define MOTOR_C_ENABLE_PIN 62

#define MOTOR_D_STEP_PIN 26     // E0 - axis
#define MOTOR_D_DIR_PIN 28
#define MOTOR_D_ENABLE_PIN 24

// motor speed and max open position
#define OPEN_ROTATION 7000L     // (recommended for DX100G Blast Gate 100mm with DRV8825 stepper driver)

// if you are using some other driver and stepper motors this value might be different

// Max amperage level for each machine
#define CIRCULAR_SAW_RUNNING 8
#define JOINTER_RUNNING 14
#define BAND_SAW_RUNNING 3.5

// Led lights PINs by gate
#define LED_PIN_A 17
#define LED_PIN_B 23
#define LED_PIN_C 25
#define LED_PIN_D 27
#define LED_BLINK_MS 75

// limit switches must read "pressed" this long before startup trusts that all gates are closed
#define SWITCH_SETTLE_MS 100

// Associate motor and gate switches
BlastGate gateA(MOTOR_A_ENABLE_PIN, MOTOR_A_STEP_PIN, MOTOR_A_DIR_PIN, OPEN_ROTATION, SWITCH_GATE_A, true);
BlastGate gateB(MOTOR_B_ENABLE_PIN, MOTOR_B_STEP_PIN, MOTOR_B_DIR_PIN, OPEN_ROTATION, SWITCH_GATE_B, true);
BlastGate gateC(MOTOR_C_ENABLE_PIN, MOTOR_C_STEP_PIN, MOTOR_C_DIR_PIN, OPEN_ROTATION, SWITCH_GATE_C, true);
BlastGate gateD(MOTOR_D_ENABLE_PIN, MOTOR_D_STEP_PIN, MOTOR_D_DIR_PIN, OPEN_ROTATION, SWITCH_GATE_D, true);

// Set max amperage levels for each machine
MachineMonitor jointer("Jointer", JOINTER_PIN, JOINTER_RUNNING);
MachineMonitor bandSaw("Band saw", BAND_SAW_PIN, BAND_SAW_RUNNING);
MachineMonitor circularSaw("Circular saw", CIRCULAR_SAW_PIN, CIRCULAR_SAW_RUNNING);

// Optional gate light indicators 
// Closed = Led on
// Moving = Led blinking
// Open   = Led off
BlinkLight gateLightA(LED_PIN_A, LED_BLINK_MS);
BlinkLight gateLightB(LED_PIN_B, LED_BLINK_MS);
BlinkLight gateLightC(LED_PIN_C, LED_BLINK_MS);
BlinkLight gateLightD(LED_PIN_D, LED_BLINK_MS);


/** HELPER METHODS / alter according to your setup **/
/** Jointer line gates are OPEN, **/
bool jointerGatesOpen() {
  return gateA.isClosed() && 
         gateB.isOpen() && 
         gateC.isClosed() && 
         gateD.isOpen();
}

bool bandSawGatesOpen() {
  return gateA.isClosed() && 
         gateB.isOpen() && 
         gateC.isOpen() && 
         gateD.isClosed();
}

bool circularSawGatesOpen() {
  return gateA.isOpen() && 
         gateB.isClosed();
}

/** All gates are closed (needed on startup to)  **/
bool allGatesClosed() {
  return gateA.isClosed() && 
         gateB.isClosed() &&
         gateC.isClosed() &&
         gateD.isClosed();
}

/** Gate are not moving (each gate is either fully open or closed) **/
bool gatesAreNotMoving() {
  return !gateA.isMoving() && 
         !gateB.isMoving() && 
         !gateC.isMoving() && 
         !gateD.isMoving();
}

/** add here all MachineMonitor listeners **/
void sampleConsumption() {
  jointer.sample();
  bandSaw.sample();
  circularSaw.sample();
}

/** no machine is running (cunsumption is 'zero') **/
bool noMachineRunning() {
  return !jointer.isRunning() && 
         !circularSaw.isRunning() && 
         !bandSaw.isRunning();
}

void updateLights() {

  if (gateA.isClosed()) {
    gateLightA.on();
  }
  else if (gateA.isMoving()) {
    gateLightA.blink();
  }
  else {
    gateLightA.off();
  }

  //
  if (gateB.isClosed()) {
    gateLightB.on();
  }
  else if (gateB.isMoving()) {
    gateLightB.blink();
  }
  else {
    gateLightB.off();
  }

  //
  if (gateC.isClosed()) {
    gateLightC.on();
  }
  else if (gateC.isMoving()) {
    gateLightC.blink();
  }
  else {
    gateLightC.off();
  }

  //
  if (gateD.isClosed()) {
    gateLightD.on();
  }
  else if (gateD.isMoving()) {
    gateLightD.blink();
  }
  else {
    gateLightD.off();
  }

  gateLightA.update();
  gateLightB.update();
  gateLightC.update();
  gateLightD.update();
}


#if DEBUG
void debugGate(const char* name, BlastGate &gate) {
  debug(name);
  debug(" pos=");
  debug(gate.getPosition());
  debug(gate.isClosed() ? " switch=PRESSED" : " switch=free");
  debug(gate.isOpen() ? " open" : "");
  debug(gate.isMoving() ? " moving" : "");
  debug(" | ");
}

/** prints position / state of all gates (target open position is -OPEN_ROTATION for inverted gates) */
void debugGates() {
  debugGate("A", gateA);
  debugGate("B", gateB);
  debugGate("C", gateC);
  debugGate("D", gateD);
  debugln("");
}
#endif

void setup() {

  if (DEBUG) {
    Serial.begin(9600);
    debugln("Starting up GateKeeper ... ");
  }

  // Wait until measurements get down low levels (no machine should be running)
  sampleConsumption();

  // close all gates (initialize to 0)
  // A limit switch can bounce while it is being pressed, so a single "closed" reading is not trusted:
  // all switches must stay pressed for SWITCH_SETTLE_MS, otherwise the gates keep closing
  while (true) {

    if (allGatesClosed() && noMachineRunning()) {
      delay(SWITCH_SETTLE_MS);
      if (allGatesClosed()) {
        break;
      }
    }

    updateLights();

    if (!allGatesClosed()) {
      
      gateA.close();
      gateB.close(); 
      gateC.close();
      gateD.close();
      
      if (allGatesClosed()) {
        debugln("Gates initialized - all gates are closed");                  
      }
    }

    if (!noMachineRunning()) {      
      sampleConsumption();
      if (noMachineRunning()) {
        debugln("Energy monitoring initialized");
      }
    }
  }

  // all switches are pressed -> this is position 0 for every gate
  gateA.resetPosition();
  gateB.resetPosition();
  gateC.resetPosition();
  gateD.resetPosition();

  debugln("Initialization finished");
#if DEBUG
  debugGates();
#endif
}


void loop() {

  updateLights();

  // measure current only when gates are not in movement
  // otherwise the gates will move very slow as measuring takes a lot of time
  if (gatesAreNotMoving()) {
  
    sampleConsumption();

    /* This part is only for logging and debugging purposes */
    if (jointer.isRunning() && !jointerGatesOpen()) {
      debugln("Jointer running - trigger open B / D, close A / C");
    }

    if (circularSaw.isRunning() && !circularSawGatesOpen()) {
      debugln("Circular saw running - trigger open A, close B");
#if DEBUG
      debugGates();
#endif
    }

    if (bandSaw.isRunning() && !bandSawGatesOpen()) {
      debugln("Band saw running - trigger open B / C, close A / D");
    }
  }

  // The logic - set position of each gate if a machine is running
  // NOTE: It is assumed that only one machine is running!
  if (circularSaw.isRunning()) {
    gateA.open();
    gateB.close();
  }
  
  if (jointer.isRunning()) {
    gateA.close();
    gateB.open();
    gateC.close();
    gateD.open();
  }
  
  if (bandSaw.isRunning()) {
    gateA.close();
    gateB.open();
    gateC.open();
    gateD.close();
  }
}