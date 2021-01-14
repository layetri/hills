#pragma once
#include <cmath>
#include "Arduino.h"

class LFO {
  public:
    LFO(char shape, float rate, int samplerate);
    ~LFO();

    void set(float rate, float shape_index, float multiplier, float pulse_width);

    void setShape(int shape);
    void setRate(float rate);
    void setMultiplier(float multiplier);
    void setPulseWidth(float pulse_width);

    void incrementPhase();
    void calculatePhaseStep();

    float getSample();
  private:
    //char shapes[6] = {"sine", "tri", "square", "saw", "ramp", "random"};
    int shape;
    int samplerate;
    float rate;
    float multiplier;
    float pulse_width;

    double phase;
    float phase_step;
    float sample;
};