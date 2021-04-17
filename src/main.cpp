#include "Header/main.h"

// Enable debugging mode
 #define DEVMODE
#define REVISION 2

unsigned long last_time = micros();
unsigned long new_time;
float interval = 1000000.0 / 44100;

long blink_time = micros();

void setup() {
  #ifdef DEVMODE
    Serial.begin(9600);
    Serial.println("Starting setup.");
  #endif

  // Initialize the DAC
  SPI.begin();

  // Turn off debounce for CV trigger inputs
  trigger_a.setDebounce(false);
  trigger_b.setDebounce(false);

  #ifdef DEVMODE
    Serial.println("Setup done. Starting loop.");
  #endif
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
    int value_a = 2047;
    int value_b = 2047;

    checkButtons();
    setLeds();

    // LFO mode
    if (mode == 0) {
      lfo.set(knobs[0].getValue(), knobs[1].getValue(), knobs[2].getValue(), knobs[3].getValue());

      value_a = (int) ((lfo.getSample() + 1.0) * 2048);
      value_b = value_a;
    }
    // Envelope mode
    else if (mode == 1) {
      env_a.set(knobs[0].getValue(), knobs[1].getValue(), knobs[2].getValue(), knobs[3].getValue());
      env_b.set(knobs[0].getValue(), knobs[1].getValue(), knobs[2].getValue(), knobs[3].getValue());

      handleTriggers();


      value_a = (int) ((env_a.getSample() + 1.0) * 2048);
      value_b = (int) ((env_b.getSample() + 1.0) * 2048);
    }
    // Line mode
    else if (mode == 2) {
      value_a = 2047;
      value_b = 2047;
    }
    // Random Sequencer mode (a la MTM's Turing Machine)
    else if (mode == 3) {
      rs.set(knobs[0].getValue(), knobs[1].getValue(), knobs[2].getValue(), knobs[3].getValue());

      handleTriggers();

      if(rs.checkMode()) {
        rs.tick();
      }

      int val = rs.getSample();


      value_a = val;
      value_b = value_a;
    }

    #if REVISION == 1
      out_a.amplitude((value_a / 2048.0) - 1.0);
      out_b.amplitude((value_b / 2048.0) - 1.0);
    #else
      DAC.Set(value_a, value_b);
    #endif


    last_time = new_time;
  }
}
