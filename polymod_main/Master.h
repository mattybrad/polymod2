#ifndef Master_h
#define Master_h
#include "Arduino.h"
#include "VirtualModule.h"

class Master : public VirtualModule {
	public:
		Master();
		~Master();
		virtual void update();
	private:
		
};

#endif
