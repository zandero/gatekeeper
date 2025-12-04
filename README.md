# Arduino-Controlled Automated Dust Blast Gates for Woodworkers

This project aim is to assist fellow woodworkers in creating their own cost-effective automated blast gates.

> **NOTE: This is quite an involved build.**  
> Apart from the hardware, you will also need:
> - to 3D print some parts (or design your own)   
> - Solder and connect all parts together
> - Measure current/voltage of the stepper motor drivers to trim them  
> - Modify the main.cpp code to fit your setup (see below)

# How does it work / operation
The Arduino board 
- Monitors the operating status of machines through a current measuring sensor.
- Activates/deactivates the relevant blast gate(s) based on the specific machine in use - i.e. signals the stepper motors to open/close a gate.
- Maintain the gate(s) in the opened or closed position until another machine is powered on.

## Capabilities

- Control up to 5 individual blast gates
- Monitor 5+ machines (limited by the number of analog inputs)

## Required hardware

> Excluded the dust collection system and manual gates

### Control centre:

- Arduino Mega 2560
- Ramps 1.4 shield (or later)
- DC 12V 10A power supply
- Enclosure for Arduino and power supply
- Master on/off mains switch (220V)

#### Voltage regulation (for AC sensor)

- 1 x Burden resistor 18 Ohms if supply voltage is 3.3V, or 33 Ohms if supply voltage is 5V
- 2 x 10k Ohm resistors (or any equal value resistor pair up to 470k Ohm)
- 1 x 10uF capacitor

### Dust gate (per gate):

- Nema 17 stepper motor
- DRV8825 (highly recommended) alternatively A4988 (not recommended might run very hot)
- 3.5mm DC audio plug (male/female) to connect blast gate switches to Arduino
- Micro Limit Switch

### Machine monitoring (per machine):

- SCT-013-000 Non-invasive AC Sensor with Split Core Transformer 100A
- 3.5mm DC audio plug (male/female) to extend reach of sensor

### Additional small hardware
- Stepper motor control wires
- Double core wire (to connect sensors and gate switches with Arduino) (shielded if possible)
- M3x6 thread inserts for 3D printing
- M4x6 thread inserts for 3D printing
- M3 and M4 screws for mounting
- self-tapping screws (various sizes)
- zip ties (a lot in various sizes)
- Soldering wire / heat shrinks ...

## Estimated cost (minimal)

> Costs were estimated on Feb. 2024

| Item                                 | Approximate price                                 |
|--------------------------------------|---------------------------------------------------|
| Arduino Mega                         | 38 EUR                                            |
| Ramps 1.4                            | 9 EUR                                             | 
| DRV8825 Step Motor Driver Module     | 15 EUR pack of 5 (3 EUR per driver)               |
| voltage regulation components        | 5 EUR                                             |
| DC 12V 10A power supply              | 20 EUR                                            |
|                                      |                                                   |
| Nema 17 stepper motor (1.7A or more) | 35 EUR pack of 5 (7 EUR per motor)                |
| Micro Limit Switch                   | 16 EUR pack of 10 (1.6 EUR per switch)            |
| Stepper motor cables (2m)            | 10 EUR pack of 6 (1.5+ EUR per cable)             |
|                                      |                                                   |
| 3.5 audio plugs                      | 9 EUR pack of 10 (1 EUR per plug)                 |
| SCT-013-000 AC sensor                | 10 EUR                                            |
|                                      |                                                   |
| **Total minimal cost**:              | 71 EUR + (14 EUR per gate) + (11 EUR per machine) |

> **Example**  
> For a setup with 4 gates and 3 machines the total minimal cost would be around **160EUR**
>
> Estimate additional 10-20% for material like wires, heat shrinks, screws, etc.
> 
> Costs can vary vastly depending on where you source your materials (this is just a ballpark estimation) 

## Limitations

The main limitation of this setup is the distance between the blast gate and the control centre (i.e. Arduino board)

I have been only able to find stepper motor cables that are 2m long - limiting the max distance between gates to 4m (13
feet). One can probably make your own stepper motor cables that are longer - not sure what the maximum length could be.

The machine monitoring cables run im my case up to 10m long without any issues.

# Getting started (the software)

Install all needed software:
- [Download and install git](https://git-scm.com/downloads) 
- checkout project from GitHub  
  > git clone https://github.com/zandero/gatekeeper.git
- install [Visual Studio Code](https://code.visualstudio.com/download)
- add [PlatformIO](https://platformio.org/install/ide?install=vscode) plugin to `Visual Studio Code`

## Build the project



## Setup main.cpp 
In order to make things work correctly you will need to modify the main logic in [main.cpp](./GateKeeper/src/main.cpp) according to your setup

- define energy monitoring pin(s) (`JOINTER_PIN`, `CIRCULAR_SAW_PIN`, ...)
- define the voltage/amperage limit when gate should move (`MOTOR_A_STEP_PIN`, `MOTOR_A_DIR_PIN`, `MOTOR_A_ENABLE_PIN`, ...) 
- define gate close switch and motor pin(s) (`SWITCH_GATE_A`, ...), 

### Alter helper functions according to your setup


# The logic

## setup()

In the `setup()` step the gates and energy monitoring is initialized.   
The code "waits" until all:
- energy monitors report a value below the given threshold (i.e. all machine must be turned off)
- and all gates report that they are closed (all gete close switches are engaged)

## loop()

Once the initial state (all gates closed, no machine running) is detected the code "monitors" each machine consumption.  
Once a spike in consumption is detected (i.e. a machine is running) the appropriate gates are opened/closed.

> NOTE: once gates are in movement energy monitoring is suspended. 
> This is due to the very slow operation of consumption check. 
> Only when all gates finish their move the energy monitoring is resumed.

# The hardware

## #1 Current/voltage detection
In order to accurately detect if a machine is running the output signal from the CT sensor needs to be conditioned, so it 
meets the input requirements of the Arduino analog inputs, i.e. a positive voltage between 0V and the ADC reference voltage.

[See this guide](https://docs.openenergymonitor.org/electricity-monitoring/ct-sensors/how-to-build-an-arduino-energy-monitor-measuring-current-only.html)

Assuming only one machine will be running at once / multiple sensors can be connected to the same circuit.   
If you are planing to run multiple machines simultaneously you will need to provide on circuit for each sensor.

## #2 Defining motor rotation and travel
### Stepper motor rotation

### Tuning the stepper drivers
If the stepper motors can't push the gate to close/open (they are skipping steps) we might need to boost the stepper driver voltage.
This will happen if the gates are mounted in such way that the motors must also resist the force of gravity i.e. gates are mounted vertically.

To find out the maximum allowed voltage you need to consired the rated current of your stepper motor.  
See the following guides:
- [general on how to do this](https://www.youtube.com/watch?v=BV-ouxhZamI)  
- [guide for DRV8825](https://lastminuteengineers.com/drv8825-stepper-motor-driver-arduino-tutorial/)

> Example: for a **1.7A** NEMA motor you will shoot for **0.85V** 

## #3 Defining machine consumption threshold 

## #4 Build a simple one gate one machine prototype
Before venturing on a full build it is recommended to check if every component is working as expected by building a simple prototype.


## #5 Putting it all together 


Putting it all together