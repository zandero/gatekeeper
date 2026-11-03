#ifndef MACHINEMONITOR_h
#define MACHINEMONITOR_h

#define CURRENT_CALIBRATION 111.1
#define CURRENT_CALCULATE 1480

// DEBUG is set via build_flags in platformio.ini (applies to src/ and lib/)
#ifndef DEBUG
  #define DEBUG 0
#endif

// how often each monitor prints its consumption (when DEBUG is on)
#define MONITOR_LOG_INTERVAL_MS 1000

#include <EmonLib.h>  

class MachineMonitor {
  public:
    MachineMonitor(const char* _name, uint8_t _monitorPin, double _machineOnCurrent);

    void sample();

    bool isRunning();

    // last measured current (Amps RMS) and configured running threshold
    double getCurrent();
    double getThreshold();
    const char* getName();

  protected:
    EnergyMonitor monitor;
    const char* name;
    double machineOnCurrent;
    double lastCurrent = 0;
    bool running = false;

#if DEBUG
    unsigned long lastLog = 0;
    void log();
#endif
};

#endif
