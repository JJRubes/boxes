#include<iostream>
#include<string>
#include<vector>

#include"Pin.hpp"
#include"CallBox.hpp"

CallBox::CallBox(std::string n, std::vector<Pin> p) {
  name = n;
  pins.swap(p);
}

void CallBox::print() {
  std::cout << "Call:" << std::endl;
  std::cout << "name: " << name << std::endl;
  std::cout << "pins:" << std::endl;
  for(Pin p : pins) {
    p.print();
  }
}
