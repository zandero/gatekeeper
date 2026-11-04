# TestOpenCloseGate

> **For gate open/close testing only.** This is not the GateKeeper firmware. It does not monitor
> machines or control the dust collector. Do not leave it running on a working installation.

This small sketch drives a single blast gate through the shared
[`BlastGate`](../GateKeeper/lib/BlastGate/) library. Use it to check that the gate's
stepper motor, driver, motor direction and closed switch are wired and configured correctly.

## Test every gate before you continue

**Run this test on each gate on its own** before you upload the main
[GateKeeper](../GateKeeper/) firmware. A gate that turns the wrong way or has a faulty closed
switch can jam or overrun the gate. Fix it here, one gate at a time, before all gates are
controlled together.

## What it does

The sketch moves the gate fully closed and fully open, again and again:

1. **Close.** It moves the gate closed until the closed switch is pressed. That point becomes
   position `0`, so the gate can start partly open.
2. **Pause** for `PAUSE_MS` (2 s).
3. **Open.** It moves the gate `OPEN_ROTATION` steps to fully open.
4. **Pause**, then go back to step 1.

When the gate reaches each end, the sketch prints a line to the serial monitor (9600 baud), for
example `Cycle 3: gate is OPEN pos=-1425`. The number counts open/close cycles, and `pos` is
negative when `INVERT_MOTOR_DIRECTION` is `true`. Nothing is printed while the gate is moving,
because printing slows the motor down. If the motor stops while closing but the switch was never
pressed, it prints a warning to check the switch.

## Procedure (repeat for each gate)

1. **Close the gate by hand** before you power on (recommended). The sketch closes the gate first
   anyway. Starting closed means a faulty switch can't drive the gate far past its closed end.
2. **Set the pins** in `src/main.cpp` to match the gate under test. They must match the
   values in the GateKeeper firmware:

   | Gate | Switch | Step | Dir | Enable | Driver slot |
   |------|--------|------|-----|--------|-------------|
   | A    | 14     | 54   | 55  | 38     | X           |
   | B    | 15     | 60   | 61  | 56     | Y           |
   | C    | 18     | 46   | 48  | 62     | Z           |
   | D    | 19     | 26   | 28  | 24     | E0          |

   Update `SWITCH_GATE_A`, `MOTOR_A_STEP_PIN`, `MOTOR_A_DIR_PIN` and `MOTOR_A_ENABLE_PIN`.

3. **Build and upload** with PlatformIO (`pio run -t upload`) and open the serial monitor
   (`pio device monitor -b 9600`). Serial messages are on by default here (`build_flags = -D DEBUG=1`
   in `platformio.ini`). Leave them on, because the checks below read them.
4. **Check the result:**
   - On the first close the gate stops on the switch, and the log shows
     `Cycle 0: gate is CLOSED pos=0`. If you see the "closed switch is not pressed" warning, or the
     gate moves away from closed, **cut power immediately**. Check the switch wiring and position,
     or flip `INVERT_MOTOR_DIRECTION` if the motor turns the wrong way. Use the same
     `INVERT_MOTOR_DIRECTION` value in the GateKeeper firmware.
   - The gate opens smoothly and stops fully open (`gate is OPEN`).
     If it doesn't open fully, or opens too far, adjust `OPEN_ROTATION`. The default is 1425 steps,
     set for a DX100G 100 mm gate with a DRV8825 driver.
   - Let it run for several cycles. Every close must end on the switch with `pos=0`, and every
     open must end at the same position.
5. Move on to the next gate only when this one passes.

When every gate has passed, upload the [GateKeeper](../GateKeeper/) firmware.
