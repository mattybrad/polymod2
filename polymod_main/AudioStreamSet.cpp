#include "Arduino.h"
#include "AudioStreamSet.h"

AudioStreamSet::AudioStreamSet() {

}

void AudioStreamSet::addInput(AudioStreamSet *setToAdd) {
	inputs[numInputs] = setToAdd;
	numInputs ++;
	Serial.print("Set ");
	Serial.print(ref);
	Serial.print(initRand);
	Serial.print(" now has ");
	Serial.print(numInputs);
	Serial.println(" inputs");
	Serial.print("INPUTS: ");
	for(int i=0;i<numInputs;i++) {
		Serial.println(inputs[i]->ref);
	}
}

void AudioStreamSet::removeInput(AudioStreamSet *setToRemove) {

}
