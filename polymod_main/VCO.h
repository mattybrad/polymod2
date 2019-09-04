#ifndef VCO_h
#define VCO_h
#include "Arduino.h"
#include "VirtualModule.h"
#include "Constants.h"

class VCO : public VirtualModule {
	public:
		VCO();
		~VCO();
		virtual void update();
	private:
		AudioSynthWaveform _oscSaw[MAX_POLYPHONY];
		AudioSynthWaveform _oscSquare[MAX_POLYPHONY];
		AudioSynthWaveform _oscTriangle[MAX_POLYPHONY];
		AudioSynthWaveform _oscSine[MAX_POLYPHONY];
		AudioStreamSet _oscSawSet;
		AudioStreamSet _oscSquareSet;
		AudioStreamSet _oscTriangleSet;
		AudioStreamSet _oscSineSet;
		VirtualPatchCable *_patchCable1;
		VirtualPatchCable *_patchCable2;
		VirtualPatchCable *_patchCable3;
		VirtualPatchCable *_patchCable4;
};

#endif
