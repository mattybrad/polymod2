// required for teensy audio
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// other libraries
#include <Bounce2.h>

// include classes
#include "PhysicalModule.h"
#include "PhysicalPatchCable.h"
#include "Menu.h"

// include specific modules - this part can be generated automatically
// AUTO GENERATED INCLUDE STATEMENTS GOES HERE

// define constants
#define MAX_MODULES 64
#define MAX_POLYPHONY 4
#define MAX_CABLES 200

// define pins
#define DEC_BUTTON_PIN 2
#define INC_BUTTON_PIN 3
#define YES_BUTTON_PIN 4
#define NO_BUTTON_PIN 5

// more definitions
byte moduleIDReadings[MAX_MODULES];
PhysicalModule physicalModules[MAX_MODULES]; // all physical modules
int newPatchReadings[MAX_CABLES][2];
int numNewPatchReadings;
PhysicalPatchCable *physicalPatchCables[MAX_CABLES]; // all currently connected physical patch cables
AudioControlSGTL5000 sgtl; // teensy audio board chip
AudioOutputI2S mainOutput; // teensy audio board output
AudioConnection *mainConnections[4];
AudioMixer4 mainMixer; // temporary - will need a more flexible mixer for more than 4 channels
AudioSynthWaveform testWave1;
AudioSynthWaveform testWave2;
AudioSynthWaveform testWave3;
AudioSynthWaveform testWave4;
//AudioConnection testWaveConnection1(testWave1,0,mainMixer,0);
//AudioConnection testWaveConnection2(testWave2,0,mainMixer,1);
//AudioConnection testWaveConnection3(testWave3,0,mainMixer,2);
//AudioConnection testWaveConnection4(testWave4,0,mainMixer,3);
AudioConnection mixerToOutput1(mainMixer,0,mainOutput,0);
AudioConnection mixerToOutput2(mainMixer,0,mainOutput,1);
Menu menu = Menu();

// buttons
Bounce incButton = Bounce();
Bounce decButton = Bounce();
Bounce yesButton = Bounce();
Bounce noButton = Bounce();

// serial stuff
byte nextPosition = 0;
byte currentCommand[8];
float tempFreq = 100.0;
unsigned long lastLoop;
unsigned long thisLoop;

void setup() {
  Serial1.begin(500000);
  Serial.begin(500000);
  incButton.attach(INC_BUTTON_PIN,INPUT_PULLUP);
  decButton.attach(DEC_BUTTON_PIN,INPUT_PULLUP);
  yesButton.attach(YES_BUTTON_PIN,INPUT_PULLUP);
  noButton.attach(NO_BUTTON_PIN,INPUT_PULLUP);
  incButton.interval(25);
  decButton.interval(25);
  yesButton.interval(25);
  noButton.interval(25);

  // init audio board
  AudioMemory(20);
  sgtl.enable();
  sgtl.volume(0.5);
  physicalModules[0].updateID(255); // module 0 hardwired as master module
  mainConnections[0] = new AudioConnection(*(physicalModules[0].virtualModule->sockets[0]->audioStreamSet.audioStreams[0]), 0, mainMixer, 0);
  mainConnections[1] = new AudioConnection(*(physicalModules[0].virtualModule->sockets[0]->audioStreamSet.audioStreams[1]), 0, mainMixer, 1);
  mainConnections[2] = new AudioConnection(*(physicalModules[0].virtualModule->sockets[0]->audioStreamSet.audioStreams[2]), 0, mainMixer, 2);
  mainConnections[3] = new AudioConnection(*(physicalModules[0].virtualModule->sockets[0]->audioStreamSet.audioStreams[3]), 0, mainMixer, 3);
  mainMixer.gain(0,0.1);
  mainMixer.gain(1,0.1);
  mainMixer.gain(2,0.1);
  mainMixer.gain(3,0.1);
  testWave1.begin(0.3,100,WAVEFORM_SINE);
  testWave2.begin(0.3,250,WAVEFORM_SINE);
  testWave3.begin(0.3,300,WAVEFORM_SINE);
  testWave4.begin(0.3,400,WAVEFORM_SINE);

  for(int i=0; i<MAX_CABLES; i++) {
    physicalPatchCables[i] = NULL;
  }
}

void loop() {
  while(Serial1.available()) {
    byte thisByte = Serial1.read();
    if(nextPosition==0) {
      // new command!
      currentCommand[0] = thisByte;
      if(thisByte == 0) {
        //Serial.println("loop end");
        updatePhysicalModuleList();
        updatePhysicalPatchCableList();
        numNewPatchReadings = 0;
      } else if(thisByte>0) {
        nextPosition++;
      }
    } else {
      currentCommand[nextPosition] = thisByte;
      switch(currentCommand[0]) {
        case 1:
        nextPosition++;
        if(nextPosition>6) {
          nextPosition=0;
          /*Serial.print("PATCH CONNECTION: ");
          Serial.print(currentCommand[1]);
          Serial.print("-");
          Serial.print(currentCommand[2]);
          Serial.print("-");
          Serial.print(currentCommand[3]);
          Serial.print(" to ");
          Serial.print(currentCommand[4]);
          Serial.print("-");
          Serial.print(currentCommand[5]);
          Serial.print("-");
          Serial.println(currentCommand[6]);*/
          int socketA = 64 * currentCommand[1] + 8 * currentCommand[2] + currentCommand[3];
          int socketB = 64 * currentCommand[4] + 8 * currentCommand[5] + currentCommand[6];
          newPatchReadings[numNewPatchReadings][0] = socketA;
          newPatchReadings[numNewPatchReadings][1] = socketB;
          numNewPatchReadings ++;
        }
        break;

        case 2:
        // analog reading (currently 8-bit, upgrade to 10-bit at some point)
        nextPosition++;
        if(nextPosition>4) {
          nextPosition=0;
          /*Serial.print("ANALOG READING: ");
          Serial.print(currentCommand[1]);
          Serial.print("-");
          Serial.print(currentCommand[2]);
          Serial.print("-");
          Serial.print(currentCommand[3]);
          Serial.print(": ");
          Serial.println(currentCommand[4]);*/
          if(currentCommand[1]==0&&currentCommand[2]==0&&currentCommand[3]==0) tempFreq = 100.0 + 10*currentCommand[4];
        }
        break;

        case 3:
        // module ID reading
        nextPosition++;
        if(nextPosition>3) {
          nextPosition=0;
          moduleIDReadings[(currentCommand[1]<<3)+currentCommand[2]] = currentCommand[3];
          /*Serial.print("MODULE ID READING: ");
          Serial.print(currentCommand[1]);
          Serial.print("-");
          Serial.print(currentCommand[2]);
          Serial.print(": ");
          Serial.println(currentCommand[3]);*/
        }
        break;
      }
    }
  }

  // menu button update code
  incButton.update();
  decButton.update();
  yesButton.update();
  noButton.update();
  if(incButton.fell()) {
    menu.incrementValue();
  } else if(decButton.fell()) {
    menu.decrementValue();
  } else if(yesButton.fell()) {
    menu.confirm();
  } else if(noButton.fell()) {
    menu.cancel();
  }
}

bool testRoutingDone = false;
VirtualPatchCable *testCable;
void updatePhysicalModuleList() {
  // skip position 0, reserved for master module
  for(int i=1; i<MAX_MODULES; i++) {
    physicalModules[i].updateID(moduleIDReadings[i]);
  }
  if(!testRoutingDone&&physicalModules[2].virtualModule!=NULL) {
    testCable = new VirtualPatchCable(physicalModules[2].virtualModule->sockets[0]->audioStreamSet,physicalModules[0].virtualModule->sockets[0]->audioStreamSet);
    Serial.println("added test patch cable");
    testRoutingDone = true;
  }
}

bool cableAlreadyExists[MAX_CABLES];
void updatePhysicalPatchCableList() {
  int i,j;
  for(i=0; i<MAX_CABLES; i++) {
    cableAlreadyExists[i] = false;
  }
  bool cableFound;
  for(i=0; i<MAX_CABLES; i++) {
    if(physicalPatchCables[i] != NULL) {
      cableFound = false;
      for(j=0; j<numNewPatchReadings; j++) {
        if(newPatchReadings[j][0]==physicalPatchCables[i]->socketA&&newPatchReadings[j][1]==physicalPatchCables[i]->socketB) {
          cableFound = true;
          cableAlreadyExists[j] = true;
        }
      }
      if(!cableFound) {
        // no reading for this cable any more - remove from list
        Serial.println("REMOVED PATCH CABLE");
        Serial.println(i);
        Serial.println(physicalPatchCables[i]->socketA);
        Serial.println(physicalPatchCables[i]->socketB);
        delete physicalPatchCables[i];
        physicalPatchCables[i] = NULL;
      }
    }
  }
  for(i=0; i<numNewPatchReadings; i++) {
    if(!cableAlreadyExists[i]) {
      // reading is not found in list - add new cable to list
      // find free space in list
      bool foundSpace = false;
      for(j=0; j<MAX_CABLES&&!foundSpace; j++) {
        if(physicalPatchCables[j] == NULL) {
          physicalPatchCables[j] = new PhysicalPatchCable();
          physicalPatchCables[j]->socketA = newPatchReadings[i][0];
          physicalPatchCables[j]->socketB = newPatchReadings[i][1];
          foundSpace = true;
          Serial.println("ADDED PATCH CABLE");
          Serial.println(j);
          Serial.println(physicalPatchCables[j]->socketA);
          Serial.println(physicalPatchCables[j]->socketB);
        }
      }
    }
  }
}
