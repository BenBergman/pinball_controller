#include "HID-Project.h"

#include <Encoder.h>

const int button_count = 8;

const int button_pins[button_count] = {
  2,
  3,
  4,
  5,
  6,
  7,
  8,
  9,
};

const KeyboardKeycode keys[button_count] = {
  KEY_LEFT_SHIFT,
  KEY_RIGHT_SHIFT,
  KEY_LEFT_CTRL,
  KEY_RIGHT_CTRL,
  KEY_ESC,
  KEY_1,
  KEY_3,
  KEY_RETURN,
};

bool previous_states[button_count];
bool current_states[button_count];

Encoder encoder(0, 1);
long previous_position = 0;



void setup() {
  for (int i = 0; i < button_count; i++) {
    pinMode(button_pins[i], INPUT_PULLUP);
    previous_states[i] = HIGH;
    current_states[i] = HIGH;
  }

  NKROKeyboard.begin();
  Consumer.begin();
}



void loop() {
  for (int i = 0; i < button_count; i++) {
    current_states[i] = digitalRead(button_pins[i]);
    if (current_states[i] != previous_states[i]) {
      KeyboardKeycode key = KEY_LEFT_SHIFT;
      if (current_states[i] == LOW) {
        NKROKeyboard.press(keys[i]);
      } else {
        NKROKeyboard.release(keys[i]);
      }
    }
    previous_states[i] = current_states[i];
  }

  long current_position = encoder.read();
  long difference = current_position - previous_position;
  if (difference > 3) {
    Consumer.write(MEDIA_VOLUME_UP);
    previous_position = current_position;
  } else if (difference < -3) {
    Consumer.write(MEDIA_VOLUME_DOWN);
    previous_position = current_position;
  }
}
