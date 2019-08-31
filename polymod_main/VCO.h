#ifndef VCO_h
#define VCO_h
#include "Arduino.h"
#include "VirtualModule.h"
#include <Audio.h>

class VCO : public VirtualModule {
	public:
		VCO();
		virtual void update();
	private:

};

#endif
