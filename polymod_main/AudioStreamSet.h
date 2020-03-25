#ifndef AudioStreamSet_h
#define AudioStreamSet_h
#include "Arduino.h"
#include "Constants.h"
#include <Audio.h>
#define MAX_SET_INPUTS 10

class AudioStreamSet {
  public:
    AudioStreamSet();
    AudioStream *audioStreams[MAX_POLYPHONY];
    AudioStreamSet *inputs[MAX_SET_INPUTS];
    int numInputs = 0;
    int initRand = random(0,1000);
    bool isPoly = false;
    bool polyStatusConfirmed = false;
    bool hardcodedPoly = false;
    int polyStatusCheckNum = 0;
    char ref = 'X';
    void addInput(AudioStreamSet *setToAdd);
    void removeInput(AudioStreamSet *setToRemove);
  private:


};

#endif
