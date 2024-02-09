# Arduino-Controlled Automated Dust Blast Gates for Woodworkers

This project aim is to assist fellow woodworkers in creating their own cost-effective automated blast gates.

## Operation

- Monitor the operating status of machines through a current measuring sensor.
- Activate/deactivate the relevant gate(s) based on the specific machine in use.
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

### Additional material required:

- Stepper motor control wires
- Double core wire (to connect sensors and gate switches with Arduino) (shielded if possible)
- Soldering wire / heat shrinks ...

## Estimated cost (minimal)

> Costs were estimated on Feb. 2024

| Item                             | Approximate price                                 |
|----------------------------------|---------------------------------------------------|
| Arduino Mega                     | 38 EUR                                            |
| Ramps 1.4                        | 9 EUR                                             | 
| DRV8825 Step Motor Driver Module | 15 EUR pack of 5 (3 EUR per driver)               |
| voltage regulation components    | 5 EUR                                             |
| DC 12V 10A power supply          | 20 EUR                                            |
|                                  |                                                   |
| Nema 17 stepper motor            | 35 EUR pack of 5 (7 EUR per motor)                |
| Micro Limit Switch               | 16 EUR pack of 10 (1.6 EUR per switch)            |
| Stepper motor cables (2m)        | 10 EUR pack of 6 (1.5+ EUR per cable)             |
|                                  |                                                   |
| 3.5 audio plugs                  | 9 EUR pack of 10 (1 EUR per plug)                 |
| SCT-013-000 AC sensor            | 10 EUR                                            |
|                                  |                                                   |
| **Total minimal cost**:          | 71 EUR + (14 EUR per gate) + (11 EUR per machine) |

> **Example**  
> For a setup with 4 gates and 3 machines the total minimal cost would be around **160EUR**
>
> Estimate additional 10-20% for material like wires, hear shrinks, etc.

## Limitations

The main limitation of this setup is the distance between the blast gate and the control centre (i.e. Arduino board)

I have been only able to find stepper motor cables that are 2m long - limiting the max distance between gate to 4m (13
feet).   
One can probably make their own stepper motor cables that are longer - not sure what the maximum length could be.

The machine monitoring cables run im my case up to 10m long without any issues.



