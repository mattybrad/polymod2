#ifndef VCF_h
#define VCF_h
#include "Arduino.h"
#include "VirtualModule.h"
#include "Constants.h"

class VCF : public VirtualModule {
	public:
		VCF();
		~VCF();
		virtual void update();
	private:
		AudioFilterStateVariable _filter[MAX_POLYPHONY];
		AudioStreamSet _filterSet;
		VirtualPatchCable *_patchCable1;
		VirtualPatchCable *_patchCable2;
		VirtualPatchCable *_patchCable3;
		VirtualPatchCable *_patchCable4;
		VirtualPatchCable *_patchCable5;
};

#endif
