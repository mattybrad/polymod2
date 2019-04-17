#ifndef VirtualSocket_h
#define VirtualSocket_h
#include "Arduino.h"
#include <AudioStream.h>

class VirtualSocket {
  public:
    VirtualSocket();
    void setOutput(AudioStream& stream, int connectionIndex);
    void setInput(AudioStream& stream, int connectionIndex);
    boolean isOutput();
    AudioStream *getStream();
    int getConnectionIndex();
    
  private:
    AudioStream* _stream;
    int _connectionIndex;
    int _socketType;
};

#endif
