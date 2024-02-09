#include "BlastGate.h"
#include "MachineMonitor.h"

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
#define MAX_MOTOR_SPEED 800L
#define OPEN_ROTATION 700L

// Current calculations and levels 
#define CIRCULAR_SAW_RUNNING 12
#define JOINTER_RUNNING 14
#define BAND_SAW_RUNNING 3.5

// Initialize gates, which motor us linked to witch switch
BlastGate gateA(MOTOR_A_ENABLE_PIN, MOTOR_A_STEP_PIN, MOTOR_A_DIR_PIN, OPEN_ROTATION, SWITCH_GATE_A);
BlastGate gateB(MOTOR_B_ENABLE_PIN, MOTOR_B_STEP_PIN, MOTOR_B_DIR_PIN, OPEN_ROTATION, SWITCH_GATE_B);
BlastGate gateC(MOTOR_C_ENABLE_PIN, MOTOR_C_STEP_PIN, MOTOR_C_DIR_PIN, OPEN_ROTATION, SWITCH_GATE_C);
BlastGate gateD(MOTOR_D_ENABLE_PIN, MOTOR_D_STEP_PIN, MOTOR_D_DIR_PIN, OPEN_ROTATION, SWITCH_GATE_D);

// Initialize current monitors
MachineMonitor jointer(JOINTER_PIN, JOINTER_RUNNING);
MachineMonitor bandSaw(BAND_SAW_PIN, BAND_SAW_RUNNING);
MachineMonitor circularSaw(CIRCULAR_SAW_PIN, CIRCULAR_SAW_RUNNING);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting up ... ");

  sampleConsumption();

  while (!allGatesClosed() || !noMachineRunning()) {
    if (!allGatesClosed()) {
      gateA.closeGate();
      gateB.closeGate(); 
      gateC.closeGate();
      gateD.closeGate();

      if (allGatesClosed()) {
        Serial.println("Gates initialized - all gates closed");
      }
    }

    if (!noMachineRunning()) {
      sampleConsumption();
      if (noMachineRunning()) {
        Serial.println("Energy monitoring initialized");
      }
    }
  }

  Serial.println("Initialization finished");
}

void loop() {

  // measure current only when gates are not in movement 
  // otherwise the gates will move very slow as measuring takes a lot of time
  if (gatesAreNotMoving()) {

    sampleConsumption();

    if (jointer.isRunning() && !jointerGatesOpen()) {
      Serial.println("Jointer running - trigger open B / D, close A / C");
    }

    if (circularSaw.isRunning() && !circularSawGatesOpen()) {
      Serial.println("Circular saw running - trigger open A, close B");
    }

    if (bandSaw.isRunning() && !bandSawGatesOpen()) {
      Serial.println("Band saw running - trigger open B / C, close A / D");
    }
  }
  
  if (circularSaw.isRunning()) {
    gateA.openGate();
    gateB.closeGate();
  }
  
  if (jointer.isRunning()) {
    gateA.closeGate();
    gateB.openGate();
    gateC.closeGate();
    gateD.openGate();
  }
  
  if (bandSaw.isRunning()) {
    gateA.closeGate();
    gateB.openGate();
    gateC.openGate();
    gateD.closeGate();
  }
}

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

bool allGatesClosed() {
  return gateA.isClosed() && 
         gateB.isClosed() &&
         gateC.isClosed() &&
         gateD.isClosed();
}

bool gatesAreNotMoving() {
  return !gateA.isMoving() && 
         !gateB.isMoving() && 
         !gateC.isMoving() && 
         !gateD.isMoving();
}

void sampleConsumption() {
  jointer.sample();
  bandSaw.sample();
  circularSaw.sample();
}

bool noMachineRunning() {
  return !jointer.isRunning() && 
         !circularSaw.isRunning() && 
         !bandSaw.isRunning();
}