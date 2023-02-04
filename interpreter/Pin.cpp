#include<cassert>
#include<memory>
#include<iostream>
#include<string>
#include<vector>

#include"Pin.hpp"

Pin::Pin() {
  hasBeenSet = false;
}

void Pin::set(std::shared_ptr<Pin> p) {
  set(p->getType(), p->value);
}

void Pin::set(std::string t, void* v) {
  assert(!hasBeenSet);

  type = t;

  if(type == "string") {
    std::string *newV = new std::string();
    *newV = *((std::string*)v);
    value = (void*)newV;
  } else {
    int *newV = new int();
    *newV = *((int*)v);
    value = (void*)newV;
  }
  hasBeenSet = true;
}

bool Pin::isSet() {
  return hasBeenSet;
}

std::string Pin::getType() {
  assert(hasBeenSet);
  return type;
}

void* Pin::getValue() {
  assert(hasBeenSet);
  return value;
}

Pin::~Pin() {
  // std::cout << "Begin pin deconstruction" << std::endl;
  if(hasBeenSet) {
    if(type == "string") {
      std::string *v = (std::string*)value;
      delete v;
    } else {
      int *v = (int*)value;
      delete v;
    }
  }
  // std::cout << "End pin deconstruction" << std::endl;
}
