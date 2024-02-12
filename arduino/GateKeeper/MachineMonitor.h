#ifndef MACHINEMONITOR_h
#define MACHINEMONITOR_h

#define CURRENT_CALIBRATION 111.1
#define CURRENT_CALCULATE 1480

#include <EmonLib.h>  

class MachineMonitor {
  public:
    MachineMonitor(uint8_t _monitorPin, double _machineOnCurrent);

    void sample();

    bool isRunning();

  protected:
    EnergyMonitor monitor;
    double machineOnCurrent;
    bool running = false;
};

#endif