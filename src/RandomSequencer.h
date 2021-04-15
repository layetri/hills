//
// Created by Daniël Kamp on 01/03/2021.
//

#ifndef HILLS_RANDOMSEQUENCER_H
#define HILLS_RANDOMSEQUENCER_H
#include <Arduino.h>


class RandomSequencer {
  public:
    explicit RandomSequencer(int samplerate);
    virtual ~RandomSequencer();

    void setLength(float length);
    void setBPM(float bpm);
    void setFreedom(float freedom);
    void setQuantise(float quantise);

    void set(float length, float bpm, float freedom, float quantise);
    void tick();
    void process();
    void assign();

    bool checkMode() const;
    int getSample() const;

  private:
    int sample;
    int samplerate;
    long ticks;

    // BPM: 0 - 10 triggered, 10 - 1023: 60 - 180 -> converted to samples
    int pulse_time;
    // Length 2 - 64
    int length;
    // Freedom factor between 0 and 1
    float freedom;

    // Mode: 0 triggered, 1 self-scheduled
    bool mode;

    int pattern[64];
    int position;

    // Quantisation: 0 none, 0.5 chromatic, 1 minor/major (random select)
    float quantise;
    float quantisation_factor;
    // Quantise scale: 0 major, 1 minor, 2 ..., 3 ..., 4 ..., 5 ...
    int quantise_scale;
    // Flag is set when knob first crosses 512, reset when first lower
    bool quantise_scale_set;
};


#endif //HILLS_RANDOMSEQUENCER_H
