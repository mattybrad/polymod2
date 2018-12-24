#include "Arduino.h"
#include "Menu.h"

Menu::Menu() {
  msHome.addItem("Load patch");
  msHome.addItem("Save patch");
  msHome.addItem("Configure modules");
  String patchString;
  for(int i=0; i<100; i++) {
    patchString = "Patch ";
    patchString.concat(i);
    msLoad.addItem(patchString);
    msSave.addItem(patchString);
  }
  msCableInfo.addItem("...");
  currentSet = &msHome;
}

void Menu::incrementValue() {
  currentSet->listIndex ++;
  handleValueChange();
}

void Menu::decrementValue() {
  currentSet->listIndex --;
  handleValueChange();
}

void Menu::handleValueChange() {
  if(currentSet->listIndex < 0) currentSet->listIndex = currentSet->getNumItems() - 1;
  if(currentSet->listIndex > currentSet->getNumItems() - 1) currentSet->listIndex = 0;
  displayText();
}

void Menu::confirm() {
  int id = currentSet->getID();
  if(id == MS_HOME) {
    if(currentSet->listIndex == 0) currentSet = &msLoad; 
    else if(currentSet->listIndex == 1) currentSet = &msSave;
  } else if(id == MS_LOAD) {
    // load patch
    currentSet = &msAttachCables;
  } else if(id == MS_SAVE) {
    // save patch (check if slot is free)
    if(currentSet->listIndex < 2) currentSet = &msSaveConfirm;
    else currentSet = &msHome;
  } else if(id == MS_SAVE_CONFIRM) {
    currentSet = &msHome;
  } else if(id == MS_ATTACH_CABLES) {
    currentSet = &msCableInfo;
  }
  currentSet->listIndex = 0;
  displayText();
}

void Menu::cancel() {
  int id = currentSet->getID();
  if(id == 3) currentSet = &msSave;
  else currentSet = &msHome;
  displayText();
}

void Menu::displayText() {
  Serial.println(currentSet->getTitle());
  Serial.println(currentSet->getNumItems() > 0 ? currentSet->getItem(currentSet->listIndex) : "Yes / No");
  Serial.println("");
}

