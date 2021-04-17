#pragma once
#define DEVMODE

#include "button.h"
#include "knob.h"
#include "led.h"

#include "LFO.h"
#include "Envelope.h"
#include "RandomSequencer.h"

#include <Arduino.h>
#include <MCP4922.h>
#include <SPI.h>

MCP4922 DAC(11,13,10,9);  // (MOSI,SCK,CS,LDAC)

int samplerate = 44100;

LFO lfo(0, 10.0, samplerate);

Envelope env_a(0.5, 0.5, 0.8, 0.5, samplerate);
Envelope env_b(0.5, 0.5, 0.8, 0.5, samplerate);

RandomSequencer rs(samplerate);

Trigger control_mode(3, true);
Trigger instrument_mode(2, true);

Trigger trigger_a(1, false);
Trigger trigger_b(0, false);

Knob knobs[4] = {Knob(14), Knob(15), Knob(16), Knob(17)};

LED c_mode_led(8);
LED i_mode[4] = {LED(7), LED(6), LED(5), LED(4)};

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
    #ifdef DEVMODE
      Serial.println("Control button pressed");
    #endif

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

    #ifdef DEVMODE
      Serial.println("Instrument button pressed");
    #endif

    instrument_mode.setStatus(false);
  } else if(!instrument_mode.getStatus() && im_block) {
    im_block = false;
  }
}

// Trigger check routine variables
bool hold_trigger_a = false;
bool hold_trigger_b = false;

void handle_trigger(int channel) {
  // Handle for Envelope mode
  if(mode == 1) {
    if(channel == 0) {
      env_a.trigger();
    } else if(channel == 1) {
      env_b.trigger();
    }
  }
  // Handle for Sequencer mode
  else if(mode == 3) {
    if(!rs.checkMode()) {
      rs.tick();
    }
  }
}

void handle_release(int channel) {
  // Handle for Envelope mode
  if(mode == 1) {
    if(channel == 0) {
      env_a.trigger_release();
    } else if(channel == 1) {
      env_b.trigger_release();
    }
  }
}

void handleTriggers() {
  trigger_a.detect();
  trigger_b.detect();

  bool stat_a = trigger_a.getStatus();
  bool stat_b = trigger_b.getStatus();

  if(stat_a != hold_trigger_a) {
    if(stat_a) {
      handle_trigger(0);
    } else {
      handle_release(0);
    }

    hold_trigger_a = stat_a;
  }

  if(stat_b != hold_trigger_b) {
    if(stat_b) {
      handle_trigger(1);
    } else {
      handle_release(1);
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