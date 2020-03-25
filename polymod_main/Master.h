#ifndef Master_h
#define Master_h
#include "Arduino.h"
#include "VirtualModule.h"
<<<<<<< HEAD
#include <Audio.h>

class Master : public VirtualModule {
  public:
    Master();
  private:
    AudioAmplifier _main;
=======
#include "Constants.h"

class Master : public VirtualModule {
	public:
		Master();
		~Master();
		virtual void update();
	private:
		AudioOutputI2S _mainOutput; // teensy audio board output
		AudioConnection *_mainConnections[MAX_POLYPHONY];
		AudioMixer4 _mixer1;
		AudioMixer4 _mixer2;
		AudioMixer4 _finalMixer;
		AudioConnection *_mixer1Connection;
		AudioConnection *_mixer2Connection;
		AudioConnection *_finalConnection1;
		AudioConnection *_finalConnection2;
>>>>>>> 47bf06dffeea8e06460e3808825af276b3af3bc4
};

#endif
