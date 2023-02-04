#pragma once

#include<cassert>
#include<memory>
#include<iostream>
#include<string>
#include<vector>

class Pin {
  public:
    Pin();

    void set(std::shared_ptr<Pin> p);

    void set(std::string t, void* v);

    bool isSet();
    std::string getType();
    void* getValue();
    ~Pin();
  private:
    bool hasBeenSet;
    void* value;
    std::string type;
};
