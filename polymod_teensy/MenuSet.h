#ifndef MenuSet_h
#define MenuSet_h
#include "Arduino.h"

class MenuSet {
  public:
    MenuSet(int id, String title);
    void addItem(String item);
    int getID();
    int getNumItems();
    String getItem(int i);
    String getTitle();
    int listIndex = 0;

  private:
    int _id;
    String _title;
    String _items[100];
    int _itemsInList = 0;
};

#endif
