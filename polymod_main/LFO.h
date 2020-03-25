#ifndef LFO_h
#define LFO_h
#include "Arduino.h"
#include "VirtualModule.h"
#include "Constants.h"

class LFO : public VirtualModule {
	public:
		LFO();
		~LFO();
		virtual void update();
	private:
		AudioSynthWaveform _oscSine[MAX_POLYPHONY];
		AudioStreamSet _oscSineSet;
		VirtualPatchCable *_patchCable1;
};

#endif
