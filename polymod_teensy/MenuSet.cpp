#include "Arduino.h"
#include "MenuSet.h"

MenuSet::MenuSet(int id, String title) {
  _id = id;
  _title = title;
}

void MenuSet::addItem(String item) {
  _items[_itemsInList] = item;
  _itemsInList ++;
}

int MenuSet::getID() {
  return _id;
}

int MenuSet::getNumItems() {
  return _itemsInList;
}

String MenuSet::getItem(int i) {
  return _items[i];
}

String MenuSet::getTitle() {
  return _title;
}

