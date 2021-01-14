#include "button.h"
#include "knob.h"
#include "led.h"

#include "lfo.h"
#include "envelope.h"

#include <Audio.h>

int samplerate = 44100;

LFO lfo(0, 10.0, samplerate);

Envelope env_a(0.5, 0.5, 0.8, 0.5, samplerate);
Envelope env_b(0.5, 0.5, 0.8, 0.5, samplerate);

Trigger control_mode(3, true);
Trigger instrument_mode(2, true);

Trigger trigger_a(1, false);
Trigger trigger_b(0, false);

Knob knobs[4] = {Knob(14), Knob(15), Knob(16), Knob(17)};

LED c_mode_led(12);
LED i_mode[4] = {LED(11), LED(10), LED(9), LED(8)};

AudioSynthWaveformDc out_a;
AudioSynthWaveformDc out_b;
AudioOutputI2S i2s;

AudioConnection patchCord1(out_a, 0, i2s, 0);
AudioConnection patchCord2(out_b, 0, i2s, 1);

// Mode: 0 - LFO, 1 - Envelope, 2 - Tap, 3 - Drum Synth
int mode = 0;

// Input mode: 0 - macro, 1 - out A, 2 - out B, 3 - 2/2
int input_mode = 0;

// Button check routine variables
bool cm_block = false;
bool im_block = false;

void checkButtons() {
  // Perform a detection routine for button presses
  control_mode.detect();
  instrument_mode.detect();

  if(control_mode.getStatus() && !cm_block) {
    cm_block = true;
    if(input_mode < 3) {
      input_mode++;
    } else {
      input_mode = 0;
    }

    control_mode.setStatus(false);
  } else if(control_mode.getStatus() && cm_block) {
    cm_block = false;
  }

  if(instrument_mode.getStatus() && !im_block) {
    im_block = true;
    if(mode < 3) {
      mode++;
    } else {
      mode = 0;
    }

    instrument_mode.setStatus(false);
  } else if(!instrument_mode.getStatus() && im_block) {
    im_block = false;
  }
}

// Trigger check routine variables
bool hold_trigger_a = false;
bool hold_trigger_b = false;

void handleEnvTriggers() {
  trigger_a.detect();
  trigger_b.detect();

  bool stat_a = trigger_a.getStatus();
  bool stat_b = trigger_b.getStatus();

  if(stat_a != hold_trigger_a) {
    if(stat_a) {
      env_a.trigger();
    } else {
      env_a.trigger_release();
    }

    hold_trigger_a = stat_a;
  }

  if(stat_b != hold_trigger_b) {
    if(stat_b) {
      env_b.trigger();
    } else {
      env_b.trigger_release();
    }

    hold_trigger_b = stat_b;
  }
}

void setLeds() {
  for(int i = 0; i < 4; i++) {
    if(i == mode) {
      i_mode[i].on();
    } else {
      i_mode[i].off();
    }
  }
}