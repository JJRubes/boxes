#include<iostream>
#include<string>
#include<vector>

#include"Pin.hpp"
#include"CallBox.hpp"

CallBox::CallBox(std::string n, std::vector<Pin> p, int X, int Y) {
  name = n;
  pins.swap(p);
  x = X;
  y = Y;
}

const std::vector<Pin> CallBox::getPins() {
  return pins;
}

Pin CallBox::getPin(size_t p) {
  return pins.at(p);
}

const std::string CallBox::getName() {
  return name;
}

int CallBox::getX() {
  return x;
}

int CallBox::getY() {
  return y;
}

void CallBox::print(std::size_t indentation, std::size_t indentSize) {
  std::string indent(indentation * indentSize, ' ');
  std::string additional(indentSize, ' ');
  std::cout << indent << "Call:\n";
  std::cout << indent << additional << "Name: \"" << name << "\"\n";
  if(pins.size() > 0) {
    std::cout << indent << additional << "Pins:\n";
    for(Pin p : pins) {
      p.print(indentation + 2, indentSize);
    }
  } else {
    std::cout << indent << additional << "Pins: None\n";
  }
}
