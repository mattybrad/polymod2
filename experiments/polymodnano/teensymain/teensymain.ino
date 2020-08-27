// required for teensy audio
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// modules
#include "ModuleVCO.h"
#include "ModuleLFO.h"
//#include "ModuleMIDI.h"
#include "ModuleMaster.h"

AudioControlSGTL5000 sgtl; // teensy audio board chip

Module *m[8];
VirtualPatchCable *vpc[8];
int patchIndex = 0; // temp

void setup() {
  while(!Serial);
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps

  // init audio board
  AudioMemory(50);
  sgtl.enable();
  sgtl.volume(0);
  m[0] = new ModuleVCO();
  m[1] = new ModuleLFO();
  m[2] = new ModuleMaster();
  //m[3] = new ModuleMIDI();
}

bool added1 = false;
bool added2 = false;
bool removed1 = false;

byte messageIndex = 0;
byte thisMessage[5];
void loop() {
  // send data only when you receive data:
  while (Serial.available() > 0) {
    
    // read the incoming byte:
    byte incomingByte = Serial.read();
    if(incomingByte >= 48 && incomingByte <= 57) {
      byte thisNum = incomingByte - 48;
      if(messageIndex == 0) {
        if(thisNum == 0) Serial.println("make connection");
        else if(thisNum == 1) Serial.println("break connection");
      }
      thisMessage[messageIndex] = thisNum;
      messageIndex ++;
      if(messageIndex == 5) {
        Serial.println("end message");
        messageIndex = 0;

        if(thisMessage[0] == 0) {
          // make connection
          if(m[thisMessage[1]]!=NULL&&m[thisMessage[3]]!=NULL) { // check modules exist
            if(m[thisMessage[1]]->socketOutputs[thisMessage[2]]->patchCableNum==-1&&m[thisMessage[3]]->socketInputs[thisMessage[4]]->patchCableNum==-1) {
              Serial.println("trying to create patch cable");
              vpc[patchIndex] = new VirtualPatchCable(m[thisMessage[1]]->socketOutputs[thisMessage[2]]->audioStreamSet,0,m[thisMessage[3]]->socketInputs[thisMessage[4]]->audioStreamSet,0);
              m[thisMessage[1]]->socketOutputs[thisMessage[2]]->patchCableNum = patchIndex;
              m[thisMessage[3]]->socketInputs[thisMessage[4]]->patchCableNum = patchIndex;
              patchIndex ++;
            } else {
              Serial.println("cable already connected");
            }
          } else {
            Serial.println("a module doesn't exist");
          }
        } else if(thisMessage[0] == 1) {
          // break connection
          int outputPatchNum = m[thisMessage[1]]->socketOutputs[thisMessage[2]]->patchCableNum;
          int inputPatchNum = m[thisMessage[3]]->socketInputs[thisMessage[4]]->patchCableNum;
          if(outputPatchNum == inputPatchNum && outputPatchNum != -1) {
            delete vpc[outputPatchNum];
            vpc[outputPatchNum] = NULL;
            m[thisMessage[1]]->socketOutputs[thisMessage[2]]->patchCableNum = -1;
            m[thisMessage[3]]->socketInputs[thisMessage[4]]->patchCableNum = -1;
          }
        }
      }
    }
  }
  byte pinNum = 0;
  for(byte i=0; i<8; i++) {
    if(m[i]!=NULL) {
      for(byte j=0; j<8; j++) {
        if(m[i]->analogInputs[j]!=NULL) {
          //m[i]->analogInputs[j]->setValue(analogRead(pinNum));
          pinNum ++;
        }
      }
    }
  }
  Serial.println(AudioMemoryUsageMax());
}
