#ifndef Menu_h
#define Menu_h
#include "MenuSet.h"
#include "Arduino.h"

#define MS_HOME 0
#define MS_LOAD 1
#define MS_SAVE 2
#define MS_SAVE_CONFIRM 3
#define MS_ATTACH_CABLES 4
#define MS_WRONG_MODULES 5
#define MS_CABLE_INFO 6

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
    MenuSet msHome = MenuSet(MS_HOME, "PolyMod 2.0");
    MenuSet msLoad = MenuSet(MS_LOAD, "Load patch");
    MenuSet msSave = MenuSet(MS_SAVE, "Save patch");
    MenuSet msSaveConfirm = MenuSet(MS_SAVE_CONFIRM, "Confirm save?");
    MenuSet msAttachCables = MenuSet(MS_ATTACH_CABLES, "Recreate patch?");
    MenuSet msWrongModules = MenuSet(MS_WRONG_MODULES, "Modules missing");
    MenuSet msCableInfo = MenuSet(MS_CABLE_INFO, "Attach cable");
};

#endif
