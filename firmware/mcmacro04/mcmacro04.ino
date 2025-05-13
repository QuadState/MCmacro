/*
 * MCmacro04 Firmware
 * 
 * Description:
 * This firmware is for MCmacro04
 * 
 * Author: QuadState
 * License: MIT License
 * Project URL: https://github.com/QuadState/MCmacro
 * 
 * USB descriptor handling is based on CH55xduino's HID keyboard example by Deqing Sun:
 * https://github.com/DeqingSun/ch55xduino/tree/ch55xduino/ch55xduino/ch55x/libraries/Generic_Examples/examples/05.USB/HidKeyboard
 * 
 * Media key (Consumer Control) functionality was independently implemented,
 * although a similar approach exists in CH55xduino examples.
 * 
 * Application logic and key assignments are original.
 * 
 * Created: 2025
 */

#include "src/USBHIDKeyboard.h"

#define KEY_COUNT 4
#define STABLE_THRESHOLD 5

const uint8_t keyPins[KEY_COUNT] = { 14, 15, 16, 17 };

typedef enum { RELEASED,
               PRESSED } ButtonState;

typedef struct {
  ButtonState state;
  uint8_t stableCount;
} SwitchState;

SwitchState switchStates[KEY_COUNT];

void handleKeyPress(uint8_t index) {
  switch (index) {
    case 0: // Copy (Ctrl+C)
      Keyboard_press(KEY_LEFT_CTRL);
      Keyboard_press('c');
      delay(10);
      Keyboard_releaseAll();
      break;

    case 1: // Paste (Ctrl+V)
      Keyboard_press(KEY_LEFT_CTRL);
      Keyboard_press('v');
      delay(10);
      Keyboard_releaseAll();
      break;

    case 2: // Undo (Ctrl+Z)
      Keyboard_press(KEY_LEFT_CTRL);
      Keyboard_press('z');
      delay(10);
      Keyboard_releaseAll();
      break;

    case 3: // Redo (Ctrl+Y)
      Keyboard_press(KEY_LEFT_CTRL);
      Keyboard_press('y');
      delay(10);
      Keyboard_releaseAll();
      break;
  }
}

// Initialize pins and USB
void setup(void) {
  for (uint8_t i = 0; i < KEY_COUNT; i++) {
    pinMode(keyPins[i], INPUT_PULLUP);
    switchStates[i].state = RELEASED;
    switchStates[i].stableCount = 0;
  }
  USBInit();
}

// Main loop
void loop(void) {
  for (uint8_t i = 0; i < KEY_COUNT; i++) {
    bool pressed = (digitalRead(keyPins[i]) == LOW);
    SwitchState *sw = &switchStates[i];

    if (sw->state == RELEASED) {
      if (pressed) {
        if (++sw->stableCount >= STABLE_THRESHOLD) {
          sw->state = PRESSED;
          sw->stableCount = 0;
          handleKeyPress(i);
        }
      } else {
        sw->stableCount = 0;
      }
    } else  {
      if (!pressed) {
        if (++sw->stableCount >= STABLE_THRESHOLD) {
          sw->state = RELEASED;
          sw->stableCount = 0;
        }
      } else {
        sw->stableCount = 0;
      }
    }
  }
  delay(1);
}


