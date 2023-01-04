#include<string>
#include<vector>

#include"DefinitionBox.hpp"
#include"CallBox.hpp"
#include"Connection.hpp"

class Program { // just a definition without pins
  public:
    DefinitionBox(std::string n, std::vector<DefinitionBox> defs, std::vector<CallBox> cs, std::vector<Connection> cns);
    void DefinitionBox::print(std::size_t indentation, std::size_t indentSize);

  private:
    std::string name;
    std::vector<DefinitionBox> definitions;
    std::vector<CallBox> calls;
    std::vector<Connection> connections;
};

Program::Program(std::string n, std::vector<DefinitionBox> defs,
                             std::vector<CallBox> cls, std::vector<Connection> cns) {
  name = n;
  definitions.swap(defs);
  calls.swap(cls);
  connections.swap(cns);
}

void Program::print(std::size_t indentation, std::size_t indentSize) {
  std::string indent(indentation * indentSize, ' ');
  std::string additional(indent_size, ' ');
  std::cout << indent << "Definition:\n";
  std::cout << indent << additional << "  Name: " << name << "\n";
  if(calls.size() > 0) {
    std::cout << indent << additional << "  Calls:\n";
    for(CallBox c : defBox.calls) {
      c.print(indentation + 2, indentSize);
    }
  } else {
    std::cout << indent << additional << "  Calls: None\n";
  }
  if(connections.size() > 0) {
    std::cout << indent << additional << "  Connections:\n";
    for(Connection c : connections) {
      std::cout << indent << additional << additional << "  Connection:\n";
      std::cout << indent << additional << additional << additional << "(" << pins[c.p1].id;
      std::cout << ", " << pins[c.p2].id << ")\n";
    }
  } else {
    std::cout << indent << additional << "  Calls: None\n";
  }
  if(definitions.size() > 0) {
    std::cout << indent << additional << "  Definitions:\n";
    for(DefinitionBox d : definitions) {
      d.print(indentation + 2, indentSize);
    }
  } else {
    std::cout << indent << additional << "  Definitions: None\n";
  }
}

int main() {
  const size_t indentSize = 2;
