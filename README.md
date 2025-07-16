# ESP32-S3 Digital Typewriter Firmware

This repository contains a minimal Arduino-based firmware targeting the Waveshare **ESP32-S3-LCD-1.47B** board. The goal is to provide a simple digital typewriter with the following features:

- Text editing on the built-in 1.47" TFT using a USB keyboard connected to the board
- Saving and loading notes from SPIFFS
- Sending the entire note as USB HID keyboard keystrokes to a connected phone
- All operations controlled via keyboard shortcuts (no touch or buttons)

## Building

The firmware uses the [ESP32 Arduino core](https://github.com/espressif/arduino-esp32). Import the project into the Arduino IDE or [PlatformIO](https://platformio.org/). Select **ESP32S3 Dev Module** as the board and make sure USB mode is set to **OTG**. The TFT must be configured in `TFT_eSPI/User_Setup.h` for an ST7789 display of size 172×320.

Required libraries:

- `TFT_eSPI` for display control (configured for ST7789 172x320)
- `USB` and `USBHost` from the ESP32 Arduino core

Upload the sketch in `src/main.cpp` to the device. If you prefer using `arduino-cli`, install it and run:

```bash
arduino-cli compile --fqbn esp32:esp32:esp32s3 src
```

## Keyboard Shortcuts

- **F1** – Save note to `/note.txt`
- **F2** – Load note from `/note.txt`
- **F3** – Send note to the phone as keystrokes

These shortcuts can be adapted inside `src/main.cpp`. The firmware relies solely on the USB keyboard for input—there are no physical buttons used.

This project is intended as a starting point. The code is heavily commented for ease of extension. Refer to the board's [wiki](https://www.waveshare.com/wiki/ESP32-S3-LCD-1.47B) for wiring and additional information.
