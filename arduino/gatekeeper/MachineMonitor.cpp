#include <Arduino.h>
#include <EmonLib.h>  
#include "MachineMonitor.h"

MachineMonitor::MachineMonitor(uint8_t _monitorPin, double _machineOnCurrent) {
  monitor.current(_monitorPin, CURRENT_CALIBRATION);

  machineOnCurrent = _machineOnCurrent; 
}

void MachineMonitor::sample() {
  double irms = monitor.calcIrms(CURRENT_CALCULATE); 
  running = irms > machineOnCurrent; 
}

bool MachineMonitor::isRunning() {
  return running;
}