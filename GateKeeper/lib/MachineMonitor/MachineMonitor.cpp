#include <Arduino.h>
#include <EmonLib.h>  
#include "MachineMonitor.h"

MachineMonitor::MachineMonitor(const char* _name, uint8_t _monitorPin, double _machineOnCurrent) {
  monitor.current(_monitorPin, CURRENT_CALIBRATION);

  name = _name;
  machineOnCurrent = _machineOnCurrent; 
}

void MachineMonitor::sample() {
  lastCurrent = monitor.calcIrms(CURRENT_CALCULATE);
  running = lastCurrent > machineOnCurrent;

#if DEBUG
  log();
#endif
}

bool MachineMonitor::isRunning() {
  return running;
}

double MachineMonitor::getCurrent() {
  return lastCurrent;
}

double MachineMonitor::getThreshold() {
  return machineOnCurrent;
}

const char* MachineMonitor::getName() {
  return name;
}

#if DEBUG
// Prints e.g. "[Circular saw] 0.85 A (threshold 8.00 A) running=no"
// at most once per MONITOR_LOG_INTERVAL_MS
void MachineMonitor::log() {
  if (millis() - lastLog < MONITOR_LOG_INTERVAL_MS) {
    return;
  }
  lastLog = millis();

  Serial.print("[");
  Serial.print(name);
  Serial.print("] ");
  Serial.print(lastCurrent);
  Serial.print(" A (threshold ");
  Serial.print(machineOnCurrent);
  Serial.print(" A) running=");
  Serial.println(running ? "YES" : "no");
}
#endif
