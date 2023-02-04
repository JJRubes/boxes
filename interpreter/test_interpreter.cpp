#include<cassert>
#include<memory>
#include<iostream>
#include<string>
#include<vector>

#include"Definition.hpp"
#include"Call.hpp"
#include"Pin.hpp"

void printOutputs(std::shared_ptr<Call> c) {
  if(c->outputPins.size() == 0) {
    std::cout << "No output" << std::endl;
    return;
  }

  std::cout << "Outputs: " << std::endl;
  for(std::shared_ptr<Pin> p: c->outputPins) {
    std::cout << "  " << p->getType() << ": ";
    if(p->getType() == "string")
      std::cout << *((std::string*)(p->getValue()));
    else
      std::cout << *((int*)(p->getValue()));
    std::cout << std::endl;
  }
}

int main() {
  {
    std::shared_ptr<Pin> p(new Pin());
    assert(!(p->isSet()));
  }
  std::cout << "Empty Pin Passed" << std::endl;
  std::cout << "\n";

  {
    std::shared_ptr<Pin> pstring(new Pin());
    std::string name = "name";
    pstring->set("string", &name);
    assert(pstring->getType() == "string");
    assert(*(std::string*)pstring->getValue() == "name");

    std::shared_ptr<Pin> pint(new Pin());
    int num = 100;
    pint->set("int", &num);
    assert(pint->getType() == "int");
    assert(*(int*)pint->getValue() == 100);
  }
  std::cout << "Set Pin Passed" << std::endl;
  std::cout << "\n";

  {
    std::vector<size_t> inputs;
    std::vector<size_t> outputs;
    std::shared_ptr<Call> c(new Call("test", inputs, outputs));
    assert(c->getName() == "test");
    assert(c->isSatisfied());
  }
  std::cout << "Empty Call Passed" << std::endl;
  std::cout << "\n";

  {
    std::shared_ptr<Definition> d(new Definition("o-24"));
    assert(d->getName() == "o-24");
  }
  std::cout << "Builtin Definition without run Passed" << std::endl;
  std::cout << "\n";

  {
    std::shared_ptr<Definition> p(nullptr);
    std::vector<std::shared_ptr<Call>> cs;
    std::vector<std::shared_ptr<Definition>> dfs;
    std::vector<size_t> i;
    std::vector<size_t> o;
    size_t pps = 0;
    std::shared_ptr<Definition> d(new Definition(p, cs, dfs, i, o, pps));
    assert(d->findFunction("o-1")->getName() == "o-1");
  }
  std::cout << "Definition without run Passed" << std::endl;
  std::cout << "\n";

  {
    // running constant int function from definition
    std::shared_ptr<Definition> d(new Definition("o-24"));
    std::vector<std::shared_ptr<Pin>> inputs;
    std::vector<std::shared_ptr<Pin>> outputs = d->run(inputs);
    assert(outputs.size() == 1);
    assert(*(int*)outputs[0]->getValue() == 24);
  }
  std::cout << "Constant 24 Definition Run Passed" << std::endl;
  std::cout << "\n";

  {
    // running add function from definition
    std::shared_ptr<Definition> d(new Definition("oxy-+"));

    // 2 + 3
    std::vector<std::shared_ptr<Pin>> inputs;
    inputs.push_back(std::shared_ptr<Pin>(new Pin()));
    int var1 = 2;
    inputs[0]->set("int", &var1);
    inputs.push_back(std::shared_ptr<Pin>(new Pin()));
    int var2 = 3;
    inputs[1]->set("int", &var2);

    std::vector<std::shared_ptr<Pin>> outputs = d->run(inputs);
    assert(outputs.size() == 1);
    assert(*(int*)outputs[0]->getValue() == 5);
  }
  std::cout << "Add Definition Run Passed" << std::endl;
  std::cout << "\n";

  {
    // running print function from definition
    std::shared_ptr<Definition> d(new Definition("i-print"));

    std::vector<std::shared_ptr<Pin>> inputs;
    inputs.push_back(std::shared_ptr<Pin>(new Pin()));
    std::string var = "success";
    inputs[0]->set("string", &var);

    std::cout << "vvvvvvv does this say 'success'?" << std::endl;
    std::vector<std::shared_ptr<Pin>> outputs = d->run(inputs);
    assert(outputs.size() == 0);
  }
  std::cout << "Print Definition Run Passed" << std::endl;
  std::cout << "\n";

  {
    /*
     *  +---+
     *  | 1 |
     *  +-o-+
     */

    // 1
    std::vector<size_t> inputs  = {};
    std::vector<size_t> outputs = {0};
    std::shared_ptr<Call> c(new Call("o-1", inputs, outputs));
    c->run(std::shared_ptr<Definition>(new Definition("o-1")));

    assert(*(int*)c->outputPins[0]->getValue() == 1);
  }
  std::cout << "1 Passed" << std::endl;
  std::cout << "\n";

  {
    /*
     *  +------+
     *  | 1000 |
     *  +---o--+
     */

    // 1000
    std::vector<size_t> inputs  = {};
    std::vector<size_t> outputs = {0};
    std::shared_ptr<Call> c = std::shared_ptr<Call>(new Call("o-1000", inputs, outputs));
    c->run(std::shared_ptr<Definition>(new Definition("o-1000")));

    assert(*(int*)c->outputPins[0]->getValue() == 1000);
  }
  std::cout << "1000 Passed" << std::endl;
  std::cout << "\n";

  {
    /*
     *  +-----------+
     *  |  +-----+  |
     *  |  | 256 |  |
     *  |  +--o--+  |
     *  |     |     |
     *  +-----o-----+
     */

    std::vector<std::shared_ptr<Call>> calls = {};
    std::vector<std::shared_ptr<Definition>> definitions = {};

    // 256
    {
      std::vector<size_t> inputs  = {};
      std::vector<size_t> outputs = {0};
      calls.push_back(std::shared_ptr<Call>(new Call("o-256", inputs, outputs)));
    }

    {
      std::vector<size_t> dinputs  = {};
      std::vector<size_t> doutputs = {0};
      size_t numPins = dinputs.size() + doutputs.size();
      Definition program = Definition(std::shared_ptr<Definition>(nullptr), calls, definitions, dinputs, doutputs, numPins);

      std::vector<size_t> cinputs  = {};
      std::vector<size_t> coutputs = {0};
      std::shared_ptr<Call> c(new Call("program", cinputs, coutputs));
      std::vector<std::shared_ptr<Pin>> outputPins = program.run(c->inputPins);
      c->outputPins.swap(outputPins);

      assert(c->outputPins.size() == 1);
      assert(*(int*)c->outputPins[0]->getValue() == 256);
    }
  }
  std::cout << "Nested 256 Passed" << std::endl;
  std::cout << "\n";

  {
    std::vector<std::shared_ptr<Call>> calls;
    std::vector<std::shared_ptr<Definition>> definitions;
    std::vector<size_t> inputs;
    std::vector<size_t> outputs;

    /*
     *  +---+  +---+  +---+
     *  | 1 o--x + y--o 2 |
     *  +---+  +-o-+  +---+
     *           |
     *       +---i---+
     *       | print |
     *       +-------+
     */

    {
      // 1
      std::vector<size_t> inputs  = {};
      std::vector<size_t> outputs = {0};
      calls.push_back(std::shared_ptr<Call>(new Call("o-1", inputs, outputs)));
    }

    {
      // 2
      std::vector<size_t> inputs  = {};
      std::vector<size_t> outputs = {1};
      calls.push_back(std::shared_ptr<Call>(new Call("o-2", inputs, outputs)));
    }

    {
      // +
      std::vector<size_t> inputs  = {0, 1};
      std::vector<size_t> outputs = {2};
      calls.push_back(std::shared_ptr<Call>(new Call("oxy-+", inputs, outputs)));
    }

    {
      // print
      std::vector<size_t> inputs  = {2};
      std::vector<size_t> outputs = {};
      calls.push_back(std::shared_ptr<Call>(new Call("i-print", inputs, outputs)));
    }

    {
      // pin 0: 1 to +
      // pin 1: 2 to +
      // pin 2: + to print
      size_t numPins = 3;
      Definition program = Definition(std::shared_ptr<Definition>(nullptr), calls, definitions, inputs, outputs, numPins);
      std::vector<std::shared_ptr<Pin>> inputPins; // no inputs
      program.run(inputPins);
    }
  }

  return 0;
}
