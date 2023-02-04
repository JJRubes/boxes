#include<iostream>

#include<cassert>
#include<memory>
#include<string>
#include<vector>

#include"Call.hpp"
#include"Pin.hpp"
#include"Definition.hpp"

Call::Call(std::string n, std::vector<size_t> i, std::vector<size_t> o) {
  inputs.swap(i);
  outputs.swap(o);
  inputPins = std::vector<std::shared_ptr<Pin>>();
  outputPins = std::vector<std::shared_ptr<Pin>>();
  name = n;
}

void Call::run(std::shared_ptr<Definition> func) {
  assert(inputs.size() == inputPins.size());
  std::vector<std::shared_ptr<Pin>> funcOutput = func->run(inputPins);
  outputPins.swap(funcOutput);
  assert(outputs.size() == outputPins.size());
}

std::string Call::getName() {
  return name;
}

bool Call::isSatisfied() {
  for(std::shared_ptr<Pin> i: inputPins) {
    if(!(i->isSet()))
      return false;
  }
  return true;
}
