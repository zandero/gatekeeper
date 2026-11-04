#include <Arduino.h>
#include <MachineMonitor.h>

// Debug on/off of serial messages is set in platformio.ini (build_flags = -D DEBUG=1 / -D DEBUG=0)
// so it applies to main.cpp and the libraries (MachineMonitor prints each reading once per second)
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

// Machine energy monitor (analog pin of the CT sensor, e.g. 3 = A3)
#define MACHINE_PIN 3

// Threshold (Amps) to check - machine is considered running above this value
// Set it to the value you plan to use in GateKeeper (e.g. CIRCULAR_SAW_RUNNING)
#define MACHINE_RUNNING 8

// the first readings after power on are too high (sensor input filter is settling) - ignore them
#define WARMUP_MS 5000
// print a min / max / average summary every SUMMARY_MS
#define SUMMARY_MS 10000

// Same MachineMonitor (calibration and sample count) as the GateKeeper firmware,
// so the measured values are exactly what GateKeeper will see
MachineMonitor machine("Machine", MACHINE_PIN, MACHINE_RUNNING);

// statistics for the current summary window
double minCurrent;
double maxCurrent;
double sumCurrent;
unsigned long samples;
unsigned long windowStart;

void resetWindow() {
  minCurrent = 1e9;
  maxCurrent = 0;
  sumCurrent = 0;
  samples = 0;
  windowStart = millis();
}

void setup() {
  Serial.begin(9600);
  debugln("Starting up machine consumption test ... ");
  debug("Sensor pin A");
  debug(MACHINE_PIN);
  debug(", threshold ");
  debug(MACHINE_RUNNING);
  debugln(" A");

  debugln("Warming up sensor, readings are ignored ...");
  unsigned long start = millis();
  while (millis() - start < WARMUP_MS) {
    machine.sample();
  }

  debugln("Measuring (summary every 10 s)");
  resetWindow();
}

void loop() {
  // each sample takes a while (CURRENT_CALCULATE samples of the analog input)
  machine.sample();

  double current = machine.getCurrent();
  if (current < minCurrent) minCurrent = current;
  if (current > maxCurrent) maxCurrent = current;
  sumCurrent += current;
  samples++;

  if (millis() - windowStart >= SUMMARY_MS) {
    // e.g. "SUMMARY min 0.12 A, max 0.31 A, avg 0.20 A (42 samples)"
    debug("SUMMARY min ");
    debug(minCurrent);
    debug(" A, max ");
    debug(maxCurrent);
    debug(" A, avg ");
    debug(sumCurrent / samples);
    debug(" A (");
    debug(samples);
    debugln(" samples)");

    resetWindow();
  }
}
