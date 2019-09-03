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
		AudioSynthWaveform _oscSaw[4];
		AudioSynthWaveform _oscSquare[4];
		AudioSynthWaveform _oscTriangle[4];
		AudioSynthWaveform _oscSine[4];
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
