# Lab5-06: Create Satellite Program (Step 7 — Answer)

Reference firmware for Lab5-06: HK downlink, battery-aware 3.3 V switching, and uplink command handling with accelerometer and gyroscope responses implemented.

## Web Serial Monitor

Open in **Chrome or Edge**, then **Add Port** → **Connect** (38400 baud):

**https://hepta-sat-training.github.io/HEPTA-SAT-Serial_Monitor/**

- HK lines appear every second in the output pane.
- `a` — prints `Hello HEPTA-SAT` ten times (1 s apart).
- `b` — saves battery voltages to `test.txt` on the SD card, then reads the file back over COM.
- `c` — downlinks 10 accelerometer samples (1 s apart).
- `d` — downlinks 10 gyroscope samples (1 s apart).
- `e` — optional extension (not implemented in this answer sketch).

## Firmware

Open `Lab5-06_create_satellite_program_step7_A.ino` in the Arduino IDE and upload to your board. For library and submodule setup, see [src/README.md](src/README.md).
