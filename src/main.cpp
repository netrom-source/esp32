#include <Arduino.h>
#include <TFT_eSPI.h>           // Display library for ST7789
#include <SPIFFS.h>             // Internal flash storage
#include <USB.h>                // USB device functionality
#include <USBHIDKeyboard.h>     // HID keyboard device
#include <USBHost.h>            // USB host stack for external keyboard

// Simple text editor for ESP32-S3-LCD-1.47B

TFT_eSPI tft = TFT_eSPI();      // Display object
USBHIDKeyboard hidKeyboard;     // HID keyboard for sending text to phone
USBHost usbHost;                // USB host controller
KeyboardController usbKeyboard(usbHost); // External USB keyboard

String currentText;             // In-memory text buffer
const char *NOTE_PATH = "/note.txt"; // default file location

// HID key codes used by KeyboardController
#define KEY_F1        0x3A
#define KEY_F2        0x3B
#define KEY_F3        0x3C
#define KEY_BACKSPACE 0x2A
#define KEY_RETURN    0x28

void drawText() {
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setTextWrap(true, true);
    tft.print(currentText);
}

void saveFile(const char *path) {
    File f = SPIFFS.open(path, FILE_WRITE);
    if (!f) return;
    f.print(currentText);
    f.close();
}

void openFile(const char *path) {
    File f = SPIFFS.open(path, FILE_READ);
    if (!f) return;
    currentText = f.readString();
    f.close();
    drawText();
}

void sendTextToPhone() {
    // Send entire text buffer as keystrokes to phone
    for (size_t i = 0; i < currentText.length(); ++i) {
        hidKeyboard.write(currentText[i]);
    }
}

// Handle input from external USB keyboard
void onKeyPress(int key) {
    switch (key) {
        case KEY_BACKSPACE:
            if (!currentText.isEmpty()) currentText.remove(currentText.length() - 1);
            break;
        case KEY_RETURN:
            currentText += '\n';
            break;
        case KEY_F1:
            saveFile(NOTE_PATH);
            break;
        case KEY_F2:
            openFile(NOTE_PATH);
            break;
        case KEY_F3:
            sendTextToPhone();
            break;
        default:
            if (key >= 32 && key <= 126) {
                currentText += (char)key;
            }
            break;
    }
    drawText();
}

void setup() {
    Serial.begin(115200);
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS mount failed");
    }
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);

    usbHost.begin();
    usbKeyboard.attachPress(onKeyPress); // attach callback

    USB.begin();
    hidKeyboard.begin();

    drawText();
}

void loop() {
    usbHost.Task();         // process USB host events
    delay(10);
}

