#pragma once

#include<iostream>

class Pin {
  public:
    Pin(char id, int x, int y);
    char getId();
    int x();
    int y();
    void print();
  private:
    char identifier;
    int posX, posY;
};
