#include "Header/LFO.h"
double rate_scale = (log(200) - log(0.01)) / (1024 - 0);

LFO::LFO(char shape, float rate, int samplerate) {
  this->rate = rate;
  this->samplerate = samplerate;
  this->shape = shape;
  this->pulse_width = 0.5;
  calculatePhaseStep();
}

LFO::~LFO() {}

void LFO::set(float rate, float shape_index, float multiplier, float pulse_width) {
  //setRate(exp(log(0.01) + rate_scale * (rate - 0)));
  setRate(rate);
  setShape((int) (shape_index * 6));
  setMultiplier(multiplier);
  setPulseWidth(pulse_width);
}

// Shape setter
void LFO::setShape(int n_shape) {
  shape = n_shape;
}
// Rate setter
void LFO::setRate(float n_rate) {
  rate = map(n_rate, 0., 1., 10, 200);
  calculatePhaseStep();
}
// Pulse width setter
void LFO::setPulseWidth(float n_pulse_width) {
  pulse_width = (float) ((n_pulse_width * 0.98) + 0.01);
}
// Multiplier setter
void LFO::setMultiplier(float n_multiplier) {
  multiplier = n_multiplier;
}

void LFO::incrementPhase() {
  phase += phase_step;

  if(phase > 1.0) phase -= 1.0;
}
void LFO::calculatePhaseStep() {
  phase_step = rate / samplerate;
}

float LFO::getSample() {
  if(shape != 5) {
    sample = 0.0;
  }

  // Sine wave
  if(shape == 0) {
    sample = sin(TWO_PI * phase);
  }
  // Triangle wave
  else if(shape == 1) {
    sample = asin(sin(TWO_PI * phase));
  }
  // Square wave
  else if(shape == 2) {
    if(phase < pulse_width) {
      sample = 1.0;
    } else {
      sample = -1.0;
    }
  }
  // Saw wave
  else if(shape == 3) {
    sample = 1.0 - (phase * 2.0);
  }
  // Ramp wave
  else if(shape == 4) {
    sample = -1.0 + (phase * 2.0);
  }
  // Random wave
  else if(shape == 5) {
    if(phase > 1.0 - phase_step) {
      sample = (random(65000) / 32500.0) - 1.0;
    }
  }

  incrementPhase();

  return sample * multiplier;
}

