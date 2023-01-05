#include<iostream>
#include<string>

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

const int Pin::x() {
  return posX;
}

const int Pin::y() {
  return posY;
}

Pin::DIRECTION Pin::getNormal() {
  return normal;
}

void Pin::print(std::size_t indentation, std::size_t indentSize) {
  std::string indent(indentation * indentSize, ' ');
  std::string additional(indentSize, ' ');
  std::cout << indent << "Pin:\n";
  std::cout << indent << additional << "id: '" << identifier << "'\n";
  std::cout << indent << additional << "pos: (" << posX << ", " << posY << ")\n";
  std::cout << indent << additional << "normal: ";
  switch(normal) {
    case Pin::DIRECTION::NORTH:
      std::cout << "North\n";
      break;
    case Pin::DIRECTION::SOUTH:
      std::cout << "South\n";
      break;
    case Pin::DIRECTION::EAST:
      std::cout << "East\n";
      break;
    case Pin::DIRECTION::WEST:
      std::cout << "West\n";
      break;
  }
}
