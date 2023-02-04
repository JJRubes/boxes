#ifndef DEFINITION_H
#define DEFINITION_H

#include<memory>
#include<string>
#include<vector>

class Call;

#include"Call.hpp"
#include"Pin.hpp"

class Definition {
  public:
    Definition(std::string n);

    Definition(std::shared_ptr<Definition> p,
        std::vector<std::shared_ptr<Call>> cs,
        std::vector<std::shared_ptr<Definition>> dfs,
        std::vector<size_t> i,
        std::vector<size_t> o, size_t pps);

    std::vector<std::shared_ptr<Pin>> run(std::vector<std::shared_ptr<Pin>> defInputs);
    std::shared_ptr<Definition> findFunction(std::string name);
    std::string getName();

  private:
    std::vector<std::shared_ptr<Pin>> runBuiltin(std::vector<std::shared_ptr<Pin>> defInputs);

    std::string name;
    bool builtin;
    size_t pinPoolSize;
    std::vector<std::shared_ptr<Call>> calls;
    std::vector<std::shared_ptr<Definition>> definitions;
    std::vector<size_t> inputs;
    std::vector<size_t> outputs;
    std::shared_ptr<Definition> parent;
};

#endif
