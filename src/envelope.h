#pragma once
#include "Arduino.h"

class Envelope {
  public:
    Envelope(float attack, float decay, float sustain, float release, int samplerate);
    ~Envelope();

    void set(float attack, float decay, float sustain, float release);
    void trigger();
    void trigger_release();
    float getSample();

  private:
    float attack;
    float decay;
    float sustain;
    float release;

    int samplerate;
    long sample_count;
    bool status;
    bool hold;

    float sample;
    float hold_sample;
};