# TestMachineConsumption

> **For measuring machine consumption only.** This is not the GateKeeper firmware. It does not
> move any gates.

This small sketch reads one CT sensor through the shared
[`MachineMonitor`](../GateKeeper/lib/MachineMonitor/) library and prints the measured current to
the serial monitor. It uses the same calibration and sample count as GateKeeper, so the numbers
you see are the numbers GateKeeper will see. Use them to choose the **running threshold** for each
machine, for example `JOINTER_RUNNING`, `CIRCULAR_SAW_RUNNING` or `BAND_SAW_RUNNING` in
[GateKeeper `main.cpp`](../GateKeeper/src/main.cpp).

## Measure every machine before you continue

**Run this test on each machine on its own**, with only that machine's sensor connected.
If a threshold is too low, gates move when nothing is running. If it is too high, a running
machine is never detected.

## What it does

1. **Warm-up.** For the first `WARMUP_MS` (5 s) readings are ignored, because they are
   too high while the sensor input settles.
2. **Each reading.** Once per second the library prints a line like
   `[Machine] 0.85 A (threshold 8.00 A) running=no`. `running` shows whether the reading is
   above the `MACHINE_RUNNING` threshold.
3. **Summary.** Every `SUMMARY_MS` (10 s) the sketch prints the lowest, highest and average reading
   in that window, for example `SUMMARY min 0.12 A, max 0.31 A, avg 0.20 A (42 samples)`.

## Procedure (repeat for each machine)

1. **Connect the sensor.** Clip the SCT-013 around **one** conductor (live *or* neutral) of the
   machine's supply, not around the whole cable. Around the whole cable, the two currents cancel
   out and the reading stays near 0 A. Plug the sensor into the analog input you plan to use
   in GateKeeper.
2. **Set the pin** in `src/main.cpp`: `MACHINE_PIN` is the analog input number (`3` = A3). It must
   match the GateKeeper pin for this machine, for example `JOINTER_PIN`.
3. **Build and upload** with PlatformIO (`pio run -t upload`) and open the serial monitor
   (`pio device monitor -b 9600`). Serial messages are on by default here (`build_flags = -D DEBUG=1`
   in `platformio.ini`). Leave them on, because the measurements are only shown there.
4. **Measure with the machine OFF.** Wait for at least two summaries and note the highest `max`.
   This is the sensor noise, usually well below 1 A.
5. **Measure with the machine ON, without cutting.** Let it run idle for at least two summaries and
   note the lowest `min`. This is the lowest reading a running machine gives. Leave out the first
   seconds after start-up, because the start-up spike is much higher than normal running.
6. **Choose the threshold** somewhere between the two, about halfway, and clearly above the
   OFF noise. Example: OFF max 0.3 A, ON min 6.0 A → threshold about **3 A**.
7. **Check the threshold.** Set `MACHINE_RUNNING` to the chosen value, upload again and switch the
   machine on and off a few times. `running` must change between `no` and `YES` every time,
   and must never show `YES` while the machine is off.
8. **Copy the threshold** into GateKeeper `main.cpp` for this machine (`JOINTER_RUNNING`,
   `CIRCULAR_SAW_RUNNING`, ...).

> If several sensors share one circuit, as described in the main README, measure each machine with the
> others switched off. Then also check that one machine running does not make another
> machine's sensor go above its threshold.

When every machine has a checked threshold, continue with the [GateKeeper](../GateKeeper/)
firmware.
