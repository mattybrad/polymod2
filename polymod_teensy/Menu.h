#ifndef Menu_h
#define Menu_h
#include "MenuSet.h"
#include "Arduino.h"

class Menu {
  public:
    Menu();
    void incrementValue();
    void decrementValue();
    void confirm();
    void cancel();
    void displayText();
    MenuSet *currentSet;

  private:
    void handleValueChange();
    MenuSet msHome = MenuSet(0, "PolyMod 2.0");
    MenuSet msLoad = MenuSet(1, "Load patch");
    MenuSet msSave = MenuSet(2, "Save patch");
    MenuSet msSaveConfirm = MenuSet(3, "Confirm save?");
};

#endif
