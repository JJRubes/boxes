#include<iostream>

#include"Pin.hpp"

Pin::Pin(char id, int x, int y, DIRECTION n) {
  identifier = id;
  posX = x;
  posY = y;
  normal = n;
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

Pin::DIRECTION Pin::getNormal() {
  return normal;
}

void Pin::print() {
  std::cout << "id: " << identifier << std::endl;
  std::cout << "pos: (" << posX << ", " << posY << ")" << std::endl;
}
