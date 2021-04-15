#include "Envelope.h"

Envelope::Envelope(float attack, float decay, float sustain, float release, int samplerate) {
  this->attack = attack;
  this->decay = decay;
  this->sustain = sustain;
  this->release = release;

  this->samplerate = samplerate;

  status = false;
  hold = false;
  sample = 0.0;
}

Envelope::~Envelope() {

}

void Envelope::set(float attack, float decay, float sustain, float release) {
  if(attack < 0.7) {
    this->attack = attack;
  } else {
    this->attack = (9.3 * (attack - 0.7)) + 0.7;
  }

  if(decay < 0.5) {
    this->decay = decay;
  } else {
    this->decay = (9.5 * (decay - 0.5)) + 0.5;
  }

  this->sustain = sustain;

  if(release < 0.5) {
    this->release = release;
  } else {
    this->release = (9.5 * (release - 0.5)) + 0.5;
  }
}

void Envelope::trigger() {
  sample_count = 0;
  status = true;
  hold = true;
}

void Envelope::trigger_release() {
  hold = false;
  hold_sample = sample;
}

float Envelope::getSample() {
  if(status) {
    if(hold) {
      if (sample_count < attack * samplerate) {
        sample += 1.0 / (attack * samplerate);
      } else if (sample_count < (attack + decay) * samplerate) {
        sample -= (1.0 - sustain) / (decay * samplerate);
      } else {
        sample = sustain;
      }
    } else {
      if(sample > 0.0) {
        sample -= hold_sample / (release * samplerate);
      } else {
        status = false;
        return 0.0;
      }
    }

    sample_count++;

    return sample;
  } else {
    return 0.0;
  }
}