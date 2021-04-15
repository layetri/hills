#include "Header/main.h"

// Enable debugging mode
// #define DEVMODE

unsigned long last_time = micros();
unsigned long new_time;
float interval = 1000000.0 / 44100;

long blink_time = micros();

void setup() {
  AudioMemory(4);
  Serial.begin(9600);

  // Turn off debounce for CV trigger inputs
  trigger_a.setDebounce(false);
  trigger_b.setDebounce(false);
}

void loop() {
  // Set a base timestamp
  new_time = micros();

  // Wrapper to handle micros() overflow
  if(new_time < last_time) {
    last_time = 0;
  }

  // Check timestamp against audio rate interval
  if(new_time - last_time >= interval) {
    float value_a = 0.0;
    float value_b = 0.0;

    checkButtons();
    setLeds();

    // LFO mode
    if (mode == 0) {
      lfo.set(knobs[0].getValue(), knobs[1].getValue(), knobs[2].getValue(), knobs[3].getValue());

      value_a = lfo.getSample();
      value_b = value_a;
    }
    // Envelope mode
    else if (mode == 1) {
      env_a.set(knobs[0].getValue(), knobs[1].getValue(), knobs[2].getValue(), knobs[3].getValue());
      env_b.set(knobs[0].getValue(), knobs[1].getValue(), knobs[2].getValue(), knobs[3].getValue());

      handleTriggers();

      value_a = env_a.getSample();
      value_b = env_b.getSample();
    }
    // Line mode
    else if (mode == 2) {
      value_a = 0;
      value_b = 0;
    }
    // Random Sequencer mode (a la MTM's Turing Machine)
    else if (mode == 3) {
      rs.set(knobs[0].getValue(), knobs[1].getValue(), knobs[2].getValue(), knobs[3].getValue());

      handleTriggers();

      if(rs.checkMode()) {
        rs.tick();
      }
      int val = rs.getSample();

      value_a = (float) ((val / 2048.0) - 1.0);
      value_b = value_a;
    }

    out_a.amplitude(value_a * -1.0);
    out_b.amplitude(value_b * -1.0);

    last_time = new_time;
  }
}
