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
		AudioSynthWaveformModulated _oscSaw[MAX_POLYPHONY];
		AudioSynthWaveformModulated _oscSquare[MAX_POLYPHONY];
		AudioSynthWaveformModulated _oscTriangle[MAX_POLYPHONY];
		AudioSynthWaveformModulated _oscSine[MAX_POLYPHONY];
		AudioStreamSet _oscSawSet;
		AudioStreamSet _oscSquareSet;
		AudioStreamSet _oscTriangleSet;
		AudioStreamSet _oscSineSet;
		VirtualPatchCable *_patchCable1;
		VirtualPatchCable *_patchCable2;
		VirtualPatchCable *_patchCable3;
		VirtualPatchCable *_patchCable4;
		VirtualPatchCable *_patchCableMod1;
};

#endif
