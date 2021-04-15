#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
//
// Created by Daniël Kamp on 01/03/2021.
//

//#define DEVMODE
#include "Header/RandomSequencer.h"

RandomSequencer::RandomSequencer(int samplerate) {
  this->samplerate = samplerate;

  sample = 0;
  position = 0;
  ticks = 0;

  set(0.5,0.5,0.5,0.5);
}

RandomSequencer::~RandomSequencer() = default;

// Master setter
void RandomSequencer::set(float s_length, float s_bpm, float s_freedom, float s_quantise) {
  setLength(s_length);
  setBPM(s_bpm);
  setFreedom(s_freedom);
  setQuantise(s_quantise);
};

void RandomSequencer::setLength(float len) {
  int l = map(len, 0., 1., 1, 6);
  length = (int) pow(2, l);
}

void RandomSequencer::setBPM(float bpm) {
  // Map 0-10 to Trigger-only mode
  if(bpm < 0.1) {
    mode = false;
  } else {
    mode = true;
    // Map values to BPM
    int beats = map(bpm, 0.1, 1., 60, 400);
    // Calculate step duration
    pulse_time = (int) ((samplerate * (60.0 / beats)) / 4);
  }
}

void RandomSequencer::setFreedom(float n_freedom) {
  freedom = n_freedom;
}

void RandomSequencer::setQuantise(float n_quantise) {
  quantise = n_quantise;
  // Calculate quantization factor
  quantisation_factor = (float) (quantise / 0.5);

  if(n_quantise > 0.5 && !quantise_scale_set) {
    quantise_scale = round(random(100) / 100.0);
  }
}

void RandomSequencer::tick() {
  ticks++;

  if(!mode || ticks > pulse_time) {
    #ifdef DEVMODE
      Serial.println(pulse_time);
    #endif

    process();
    ticks = 0;
  }
}

void RandomSequencer::process() {
  if(pattern[position] == 0 || (random(100) / 100.0) < freedom) {
    assign();
  }

  int smp = pattern[position];
  #ifdef DEVMODE
    Serial.print("length: "); Serial.print(length); Serial.print("\t");
    Serial.print("pulse: "); Serial.print(pulse_time); Serial.print("\t");
    Serial.print("freedom: "); Serial.print(freedom); Serial.print("\t");
    Serial.print("quantise: "); Serial.print(quantise); Serial.print("\t");
    Serial.print("value: "); Serial.print(smp); Serial.print("\t");
  #endif

  // Increment and loop the pattern position
  if(position < length - 1) {
    position++;
  } else {
    position = 0;
  }

  sample = smp;
}

// Assign a new value to the current pattern place
void RandomSequencer::assign() {
  int val = random(4096);

  // Quantise chromatically
  if(quantise < 0.5) {
    // Calculate quantised value
    //    -> 68.25 = (4095 / 5) / 12
    int quantised = val - (((val * 100) % 6825) / 100);
    // Calculate weighed average for final value
    pattern[position] = quantised * quantisation_factor + val * (1 - quantisation_factor);
  } else {
    pattern[position] = val;
  }

  // Quantise to scale
  // TODO: implement quantise to scale
}

bool RandomSequencer::checkMode() const {
  return mode;
}

int RandomSequencer::getSample() const {
  return sample;
}

#pragma clang diagnostic pop
