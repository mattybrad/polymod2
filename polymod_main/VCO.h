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
		AudioStreamSet _oscSquare;
		VirtualPatchCable *_patchCable;
};

#endif
