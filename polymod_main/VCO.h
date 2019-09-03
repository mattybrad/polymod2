#ifndef VCO_h
#define VCO_h
#include "Arduino.h"
#include "VirtualModule.h"

class VCO : public VirtualModule {
	public:
		VCO();
		~VCO();
		virtual void update();
	private:
		AudioSynthWaveform _oscSquareX[4];
		AudioSynthWaveform _oscSineX[4];
		AudioStreamSet _oscSquare;
		AudioStreamSet _oscSine;
		VirtualPatchCable *_patchCable1;
		VirtualPatchCable *_patchCable2;
};

#endif
