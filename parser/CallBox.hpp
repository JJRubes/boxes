#pragma once

#include<string>
#include<vector>

#include"Pin.hpp"

class CallBox {
  public:
    CallBox(std::string n, std::vector<Pin> p, int x, int y);
    void print(std::size_t indentation, std::size_t indentSize);

    const std::vector<Pin> getPins();
    Pin getPin(size_t p);
    const std::string getName();
    int getX();
    int getY();
  private:
    std::vector<Pin> pins;
    std::string name;
    int x, y;
};
