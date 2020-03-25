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
#include "PhysicalPatchCable.h"
#include "Menu.h"

// include Constants
#include "Constants.h"

// include specific modules - this part can be generated automatically
// AUTO GENERATED INCLUDE STATEMENTS GOES HERE

// define pins
#define DEC_BUTTON_PIN 2
#define INC_BUTTON_PIN 3
#define YES_BUTTON_PIN 4
#define NO_BUTTON_PIN 5

// more definitions
byte moduleIDReadings[MAX_MODULES];
PhysicalModule *physicalModules[MAX_MODULES]; // all physical modules
int newPatchReadings[MAX_CABLES][2];
int numNewPatchReadings;
PhysicalPatchCable *physicalPatchCables[MAX_CABLES]; // all currently connected physical patch cables
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

  for(int i=0; i<MAX_CABLES; i++) {
    physicalPatchCables[i] = NULL;
  }
  for(int i=0; i<MAX_MODULES; i++) {
    physicalModules[i] = NULL;
  }

  physicalModules[0] = new PhysicalModule(255); // module 0 hardwired as master module
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
      if(thisByte == 0) {
        updatePhysicalModuleList();
        updatePhysicalPatchCables();
        //Serial.println(AudioMemoryUsageMax());
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

void updateVirtualPatchCables() {
  Serial.println("Update virtual patch cables");
  for(int i=0; i<MAX_CABLES; i++) {
    if(physicalPatchCables[i] != NULL) {
      int socketA = physicalPatchCables[i]->physicalSocketA;
      int socketB = physicalPatchCables[i]->physicalSocketB;
      int moduleA = socketA / 8;
      int moduleSocketA = socketA % 8;
      int moduleB = socketB / 8;
      int moduleSocketB = socketB % 8;
      if(physicalModules[moduleA] != NULL && physicalModules[moduleB] != NULL) {
        physicalPatchCables[i]->update(
          physicalModules[moduleA]->virtualModule->sockets[moduleSocketA],
          physicalModules[moduleB]->virtualModule->sockets[moduleSocketB]
        );
      } else {
        Serial.println("FAIL (module didn't exist)");
      }
    }
  }
  // run recursive function to see whether each AudioStreamSet should be mono or poly
  int checkNum = 0;
  while(checkNum < 2) {
    checkPolyStatus(&physicalModules[0]->virtualModule->sockets[0]->audioStreamSet, checkNum);
    checkNum ++;
  }
}

void checkPolyStatus(AudioStreamSet *thisSet, int checkNum) {
  Serial.println("Checking poly status...");
  thisSet->polyStatusCheckNum ++;
  if(thisSet->hardcodedPoly) {
    thisSet->isPoly = true;
    thisSet->polyStatusConfirmed = true;
  }
  bool allMono = true;
  bool allInputsConfirmed = true;
  Serial.print("Num inputs for ");
  Serial.print(thisSet->ref);
  Serial.print(": ");
  Serial.println(thisSet->numInputs);
  for(int i=0;i<thisSet->numInputs;i++) {
    Serial.print("Checking input: ");
    Serial.println(thisSet->inputs[i]->ref);
    if(thisSet->inputs[i]->polyStatusCheckNum==checkNum) {
      checkPolyStatus(thisSet->inputs[i], checkNum);
    }
    if(thisSet->inputs[i]->isPoly) allMono = false;
    if(!thisSet->inputs[i]->polyStatusConfirmed) allInputsConfirmed = false;
  }
  if(!allMono) {
    thisSet->isPoly = true;
    thisSet->polyStatusConfirmed = true;
  }
  if(allInputsConfirmed) {
    thisSet->polyStatusConfirmed = true;
  }
  Serial.print("Poly status for ");
  Serial.print(thisSet->ref);
  Serial.print(": ");
  Serial.println(thisSet->isPoly?"POLY":"MONO");
}

void report_ram_stat(const char* aname, uint32_t avalue) {
  Serial.print(aname);
  Serial.print(": ");
  Serial.print((avalue + 512) / 1024);
  Serial.print(" Kb (");
  Serial.print((((float) avalue) / ram.total()) * 100, 1);
  Serial.println("%)");
};

void report_ram() {
  bool lowmem;
  bool crash;

  Serial.println("==== memory report ====");

  report_ram_stat("free", ram.adj_free());
  report_ram_stat("stack", ram.stack_total());
  report_ram_stat("heap", ram.heap_total());

  lowmem = ram.warning_lowmem();
  crash = ram.warning_crash();
  if(lowmem || crash) {
    Serial.println();

    if(crash)
      Serial.println("**warning: stack and heap crash possible");
    else if(lowmem)
      Serial.println("**warning: unallocated memory running low");
  };

  Serial.println();
};
