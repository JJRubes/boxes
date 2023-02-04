#include<cassert>
#include<memory>
#include<iostream>
#include<string>
#include<vector>

#include"Pin.hpp"
#include"Call.hpp"
#include"Definition.hpp"

Definition::Definition(std::string n) {
  name = n;
  builtin = true;
}

Definition::Definition(std::shared_ptr<Definition> p,
    std::vector<std::shared_ptr<Call>> cs,
    std::vector<std::shared_ptr<Definition>> dfs,
    std::vector<size_t> i,
    std::vector<size_t> o, size_t pps) {
  calls.swap(cs);
  definitions.swap(dfs);
  inputs.swap(i);
  outputs.swap(o);
  builtin = false;
  parent = p;
  pinPoolSize = pps;
}


std::vector<std::shared_ptr<Pin>> Definition::run(std::vector<std::shared_ptr<Pin>> defInputs) {
  if(builtin) {
    std::vector<std::shared_ptr<Pin>> out = runBuiltin(defInputs);
    return out;
  }

  // where this instance's values are stored
  std::vector<std::shared_ptr<Pin>> pinPool;
  for(size_t i = 0; i < pinPoolSize; i++) {
    pinPool.push_back(std::shared_ptr<Pin>(new Pin()));
  }

  // initialise def's inputs
  assert(inputs.size() == defInputs.size());
  for(size_t i = 0; i < inputs.size(); i++) {
    pinPool[inputs[i]]->set(defInputs[i]);
  }
  
  // to quickly find what should be run
  std::vector<bool> hasRun = std::vector<bool>(calls.size(), false);

  size_t numLeft = hasRun.size();
  while(numLeft > 0) {
    size_t prevNumLeft = numLeft;
    for(size_t i = 0; i < hasRun.size(); i++) {
      if(hasRun[i]) {
        continue;
      }

      // initialise call pins ready to be run
      assert(calls[i]->inputPins.size() <= calls[i]->inputs.size());
      for(size_t j = 0; j < calls[i]->inputs.size(); j++) {
        if(calls[i]->inputPins.size() < calls[i]->inputs.size())
          calls[i]->inputPins.push_back(std::shared_ptr<Pin>(new Pin()));
        calls[i]->inputPins[j] = pinPool[calls[i]->inputs[j]];
      }

      // if all the inputs are set run the call
      if(calls[i]->isSatisfied()) {
        calls[i]->run(findFunction(calls[i]->getName()));
        for(size_t j = 0; j < calls[i]->outputs.size(); j++) {
          pinPool[calls[i]->outputs[j]] = calls[i]->outputPins[j];
        }
        hasRun[i] = true;
        numLeft--;
      }
    }
    assert(prevNumLeft > numLeft && "Stuck in a loop");
  }

  std::vector<std::shared_ptr<Pin>> out;
  for(size_t index: outputs) {
    out.push_back(pinPool[index]);
  }

  return out;
}

std::shared_ptr<Definition> Definition::findFunction(std::string name) {
  for(std::shared_ptr<Definition> d: definitions) {
    if(d->getName() == name)
      return d;
  }
  if(parent != nullptr)
    return parent->findFunction(name);
  return std::shared_ptr<Definition>(new Definition(name));
}

std::string Definition::getName() {
  return name;
}

std::vector<std::shared_ptr<Pin>> Definition::runBuiltin(std::vector<std::shared_ptr<Pin>> defInputs) {
  if(name == "oxy-+") {
    assert(defInputs.size() == 2);
    std::vector<std::shared_ptr<Pin>> outputPins;
    outputPins.push_back(std::shared_ptr<Pin>(new Pin));
    assert(defInputs[0]->getType() == "int" && defInputs[1]->getType() == "int");
    int x = *((int*)(defInputs[0]->getValue()));
    int y = *((int*)(defInputs[1]->getValue()));
    int o = x + y;
    outputPins[0]->set("int", (void*)(&o));
    return outputPins;
  }
  if(name[0] == 'o' && name[1] == '-') {
    assert(defInputs.size() == 0);
    std::vector<std::shared_ptr<Pin>> outputPins;
    outputPins.push_back(std::shared_ptr<Pin>(new Pin));
    int num = stoi(name.substr(2, std::string::npos));
    outputPins[0]->set("int", (void*)(&num));
    return outputPins;
  }
  if(name == "i-print") {
    assert(defInputs.size() == 1);
    std::vector<std::shared_ptr<Pin>> outputPins;
    std::shared_ptr<Pin> i = defInputs[0];
    if(i->getType() == "string") {
      std::string value = *((std::string*)(i->getValue()));
      std::cout << value << std::endl;
    } else {
      int value = *((int*)(i->getValue()));
      std::cout << value << std::endl;
    }
    return outputPins;
  }

  std::vector<std::shared_ptr<Pin>> outputPins;
  return outputPins;
}
