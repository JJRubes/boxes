#include<iostream>

#include"Pin.hpp"

Pin::Pin(char id, int x, int y) {
  identifier = id;
  posX = x;
  posY = y;
}

char Pin::getId() {
  return identifier;
}

int Pin::x() {
  return posX;
}

int Pin::y() {
  return posY;
}

void Pin::print() {
  std::cout << "id: " << identifier << std::endl;
  std::cout << "pos: (" << posX << ", " << posY << ")" << std::endl;
}
