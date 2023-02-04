#ifndef CALL_H
#define CALL_H

#include<memory>
#include<string>
#include<vector>

class Definition;

#include"Definition.hpp"
#include"Pin.hpp"

class Call {
  public:
    Call(std::string n, std::vector<size_t> i, std::vector<size_t> o);

    void run(std::shared_ptr<Definition> parent);

    std::string getName();

    bool isSatisfied();

    std::vector<size_t> inputs;
    std::vector<size_t> outputs;
    std::vector<std::shared_ptr<Pin>> inputPins;
    std::vector<std::shared_ptr<Pin>> outputPins;
  private:
    std::string name;
};

#endif
