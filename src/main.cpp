
#include "main.h"

unsigned long last_time = micros();
unsigned long new_time;
float interval = 1000000.0 / 44100;

void setup() {
  AudioMemory(4);


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

    if (mode == 0) {
      lfo.set(knobs[0].getValue(), knobs[1].getValue(), knobs[2].getValue(), knobs[3].getValue());

      value_a = lfo.getSample();
      value_b = value_a;
    } else if (mode == 1) {
      env_a.set(knobs[0].getValue(), knobs[1].getValue(), knobs[2].getValue(), knobs[3].getValue());
      env_b.set(knobs[0].getValue(), knobs[1].getValue(), knobs[2].getValue(), knobs[3].getValue());

      handleEnvTriggers();

      value_a = env_a.getSample();
      value_b = env_b.getSample();
    }

    out_a.amplitude(value_a * -1.0);
    out_b.amplitude(value_b * -1.0);

    last_time = new_time;
  }
}
