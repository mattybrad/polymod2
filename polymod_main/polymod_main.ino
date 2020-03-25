// required for teensy audio
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// other libraries
#include <Bounce2.h>

// monitor RAM
#include <cstdint>
#include "RamMonitor.h"
RamMonitor ram;
uint32_t reporttime;

// include classes
#include "PhysicalModule.h"
#include "Master.h"
#include "PhysicalPatchCable.h"
#include "VirtualPatchCable.h"
#include "Menu.h"

// include Constants
#include "Constants.h"

// include specific modules - this part can be generated automatically
// AUTO GENERATED INCLUDE STATEMENTS GOES HERE

// define constants
#define MAX_MODULES 64
#define MAX_POLYPHONY 4
#define MAX_SOCKETS 512
#define MAX_CABLES 200

// define pins
#define DEC_BUTTON_PIN 2
#define INC_BUTTON_PIN 3
#define YES_BUTTON_PIN 4
#define NO_BUTTON_PIN 5

// more definitions
byte moduleIDReadings[MAX_MODULES];
PhysicalPatchCable patchCableConnections[MAX_CABLES]; // main array of physically connected patch cables
PhysicalPatchCable newPatchCableConnections[MAX_CABLES]; // most recently updated array of connected patch cables, to check for new connections/disconnections
VirtualPatchCable virtualPatchCableConnections[MAX_CABLES];
int newConnectionIndex = 0;
PhysicalModule physicalModules[MAX_MODULES]; // all physical modules
AudioControlSGTL5000 sgtl; // teensy audio board chip
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
  // init RAM reporting
  ram.initialize();
  while(!Serial);
  reporttime = millis();

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
  AudioMemory(50);
  sgtl.enable();
  sgtl.volume(0.3);

  // init test modules
  physicalModules[0].virtualModule = new Master();
  physicalModules[8].virtualModule = new TestOscillator();
}

void loop() {
  // report RAM usage every 2 seconds
  uint32_t time = millis();
  if((time - reporttime) > 2000) {
    reporttime = time;
    //report_ram();
  };
  ram.run();

  while(Serial1.available()) {
    byte thisByte = Serial1.read();
    if(nextPosition==0) {
      // new command!
      currentCommand[0] = thisByte;
      if(thisByte>0) nextPosition ++; // don't increment position for command 0 because it doesn't have any other data expected
      else {
        updatePatchCableConnections();
      }
    } else {
      currentCommand[nextPosition] = thisByte;
      switch(currentCommand[0]) {
        case 1:
        nextPosition++;
        if(nextPosition>6) {
          nextPosition=0;
          addNewPatchCableConnection((currentCommand[1]<<6)+(currentCommand[2]<<3)+currentCommand[3],(currentCommand[4]<<6)+(currentCommand[5]<<3)+currentCommand[6]);
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

  // menu button update code (probably not the best place for this, remnant from earlier code, fix later)
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

void updatePhysicalModuleList() {
  bool anyChanges = false;
  // temp - add dummy module
  moduleIDReadings[8] = 136;
  moduleIDReadings[3] = 88;
  moduleIDReadings[4] = 99;
  moduleIDReadings[2] = 0;
  // skip position 0, reserved for master module
  for(int i=1; i<MAX_MODULES; i++) {
    if(moduleIDReadings[i] == 0) {
      // no physical module present
      if(physicalModules[i]==NULL) {
        // no change
      } else {
        // module has been removed - destroy physical module object
        delete physicalModules[i];
        physicalModules[i]=NULL;
        anyChanges= true;
      }
    } else {
      // physical module present
      if(physicalModules[i]->id==moduleIDReadings[i]) {
        // no change
      } else {
        // module has been added - create new physical module object
        physicalModules[i] = new PhysicalModule(moduleIDReadings[i]);
        anyChanges = true;
      }
    }
  }
  if(anyChanges) updateVirtualPatchCables(); // possibly unnecessary? but shouldn't break anything
}

bool cableAlreadyExists[MAX_CABLES];
void updatePhysicalPatchCables() {
  bool anyChanges = false;

  // temp - adding dummy patch cable readings
  newPatchReadings[numNewPatchReadings][0] = 0;
  newPatchReadings[numNewPatchReadings][1] = 34;
  numNewPatchReadings ++;
  newPatchReadings[numNewPatchReadings][0] = 24;
  newPatchReadings[numNewPatchReadings][1] = 33;
  numNewPatchReadings ++;
  newPatchReadings[numNewPatchReadings][0] = 32;
  newPatchReadings[numNewPatchReadings][1] = 64;
  numNewPatchReadings ++;

  int i,j;
  for(i=0; i<MAX_CABLES; i++) {
    cableAlreadyExists[i] = false;
  }
  bool cableFound;
  for(i=0; i<MAX_CABLES; i++) {
    if(physicalPatchCables[i] != NULL) {
      cableFound = false;
      for(j=0; j<numNewPatchReadings; j++) {
        if(newPatchReadings[j][0]==physicalPatchCables[i]->physicalSocketA&&newPatchReadings[j][1]==physicalPatchCables[i]->physicalSocketB) {
          cableFound = true;
          cableAlreadyExists[j] = true;
        }
      }
      if(!cableFound) {
        // no reading for this cable any more - remove from list
        delete physicalPatchCables[i];
        physicalPatchCables[i] = NULL;
        anyChanges = true;
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
          physicalPatchCables[j] = new PhysicalPatchCable(newPatchReadings[i][0], newPatchReadings[i][1]);
          foundSpace = true;
        }
      }
      if(foundSpace) {
        anyChanges = true;
      }
    }
  }
  if(anyChanges) {
    updateVirtualPatchCables();
  }
}

      // initialise new virtual modules
      //if(physicalModules[i].id==136) physicalModules[i].virtualModule = new TestOscillator();
    }
  }
  // run recursive function to see whether each AudioStreamSet should be mono or poly
  int checkNum = 0;
  while(checkNum < 2) {
    checkPolyStatus(&physicalModules[0]->virtualModule->sockets[0]->audioStreamSet, checkNum);
    checkNum ++;
  }
}

void addNewPatchCableConnection(int socket1, int socket2) {
  newPatchCableConnections[newConnectionIndex].inUse = true;
  newPatchCableConnections[newConnectionIndex].socket1 = socket1;
  newPatchCableConnections[newConnectionIndex].socket2 = socket2;
  newConnectionIndex ++;
}

void updatePatchCableConnections() {
  for(int i=0;i<MAX_CABLES;i++) {
    boolean cableFound = false;
    for(int j=0;j<newConnectionIndex&&!cableFound;j++) {
      if(patchCableConnections[i].inUse) {
        if(patchCableConnections[i].socket1==newPatchCableConnections[j].socket1&&patchCableConnections[i].socket2==newPatchCableConnections[j].socket2) {
          newPatchCableConnections[j].inUse = false;
          cableFound = true;
        }
      }
    }
    if(!cableFound&&patchCableConnections[i].inUse) {
      // patch cable removed
      patchCableConnections[i].inUse = false;
      Serial.print("REMOVED ");
      Serial.print(patchCableConnections[i].socket1);
      Serial.print("->");
      Serial.println(patchCableConnections[i].socket2);
    }
  }
  for(int i=0;i<newConnectionIndex;i++) {
    if(newPatchCableConnections[i].inUse) {
      // patch cable added, find space in list
      boolean spaceFound = false;
      for(int j=0;j<MAX_CABLES&&!spaceFound;j++) {
        if(!patchCableConnections[j].inUse) {
          patchCableConnections[j].inUse = true;
          patchCableConnections[j].socket1 = newPatchCableConnections[i].socket1;
          patchCableConnections[j].socket2 = newPatchCableConnections[i].socket2;
          spaceFound = true;
          createVirtualConnectionFromPhysical(j);
          Serial.print("ADDED ");
          Serial.print(patchCableConnections[j].socket1);
          Serial.print("->");
          Serial.println(patchCableConnections[j].socket2);
        }
      }
    }
  }
  newConnectionIndex = 0;
}

void createVirtualConnectionFromPhysical(int i) {
  int socket1Module = patchCableConnections[i].socket1>>3;
  int socket2Module = patchCableConnections[i].socket2>>3;
  int socket1Pin = patchCableConnections[i].socket1 - (socket1Module<<3);
  int socket2Pin = patchCableConnections[i].socket2 - (socket2Module<<3);
  Serial.println(socket1Module);
  Serial.println(socket1Pin);
  Serial.println(socket2Module);
  Serial.println(socket2Pin);
  VirtualSocket& socket1 = physicalModules[socket1Module].virtualModule->getSocket(socket1Pin);
  VirtualSocket& socket2 = physicalModules[socket1Module].virtualModule->getSocket(socket2Pin);
  virtualPatchCableConnections[i].initialise(socket1, socket2);
}
