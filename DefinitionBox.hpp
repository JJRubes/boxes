#pragma once

#include<string>
#include<vector>

#include"CallBox.hpp"
#include"Pin.hpp"

// This should be the thing that contains everything,
// or maybe nothing??
// definitely needs some way of representing connections
class DefinitionBox {
  public:
    DefinitionBox(std::string n, std::vector<Pin> p, std::vector<DefinitionBox> defs, std::vector<CallBox> cs);

  private:
    std::string name;
    std::vector<Pin> pins;
    std::vector<DefinitionBox> definitions;
    std::vector<CallBox> calls;
};
