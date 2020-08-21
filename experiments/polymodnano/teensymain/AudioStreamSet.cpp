#include "Arduino.h"
#include "AudioStreamSet.h"

AudioStreamSet::AudioStreamSet() {

}

void AudioStreamSet::addInput(AudioStreamSet *setToAdd) {
  inputs[numInputs] = setToAdd;
  numInputs ++;
}

void AudioStreamSet::removeInput(AudioStreamSet *setToRemove) {

}
