#pragma once

#include<string>
#include<vector>

#include"Pin.hpp"

class CallBox {
  public:
    CallBox(std::string n, std::vector<Pin> p);
    void print();

  private:
    std::string name;
    std::vector<Pin> pins;
};
