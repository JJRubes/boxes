#include<iostream>
#include<string>
#include<vector>

#include"DefinitionBox.hpp"
#include"CallBox.hpp"
#include"Connection.hpp"
#include"Pin.hpp"

DefinitionBox::DefinitionBox(std::string n,
                             std::vector<Pin> p,
                             std::vector<DefinitionBox> defs,
                             std::vector<CallBox> cls,
                             std::vector<Connection> cns,
                             int X, int Y) {
  name = n;
  pins.swap(p);
  definitions.swap(defs);
  calls.swap(cls);
  connections.swap(cns);
  x = X;
  y = Y;
}

const std::vector<Pin> DefinitionBox::getPins() {
  return pins;
}

Pin DefinitionBox::getPin(size_t p) {
  return pins.at(p);
}

const std::string DefinitionBox::getName() {
  return name;
}

int DefinitionBox::getX() {
  return x;
}

int DefinitionBox::getY() {
  return y;
}

void DefinitionBox::print(std::size_t indentation, std::size_t indentSize) {
  std::string indent(indentation * indentSize, ' ');
  std::string additional(indentSize, ' ');
  std::cout << indent << "Definition:\n";
  std::cout << indent << additional << "Name: " << name << "\n";
  if(pins.size() > 0) {
    std::cout << indent << additional << "Pins:\n";
    for(Pin p : pins) {
      p.print(indentation + 2, indentSize);
    }
  } else {
    std::cout << indent << additional << "Pins: None\n";
  }
  if(calls.size() > 0) {
    std::cout << indent << additional << "Calls:\n";
    for(CallBox c : calls) {
      c.print(indentation + 2, indentSize);
    }
  } else {
    std::cout << indent << additional << "Calls: None\n";
  }
  if(connections.size() > 0) {
    std::cout << indent << additional << "Connections:\n";
    for(Connection c : connections) {
      std::cout << indent << additional << additional << "  Connection:\n";
      std::cout << indent << additional << additional << additional << "(";
      if(c.t1 == Connection::BoxType::SELF) {
        std::cout << "self." << pins[c.p1].getId();
      } else if(c.t1 == Connection::BoxType::CALL) {
        std::cout << "calls." << calls[c.b1].getName() << "." << calls[c.b1].getPin(c.p1).getId();
      } else {
        std::cout << "definitions." << definitions[c.b1].getName() << "." << definitions[c.b1].getPin(c.p1).getId();
      }
      std::cout << ",\n";
      std::cout << indent << additional << additional << additional << additional;
      if(c.t2 == Connection::BoxType::SELF) {
        std::cout << "self." << pins[c.p2].getId();
      } else if(c.t2 == Connection::BoxType::CALL) {
        std::cout << "calls." << calls[c.b2].getName() << "." << calls[c.b2].getPin(c.p2).getId();
      } else {
        std::cout << "definitions." << definitions[c.b2].getName() << "." << definitions[c.b2].getPin(c.p1).getId();
      }
      std::cout << ")\n";
    }
  } else {
    std::cout << indent << additional << "Connections: None\n";
  }
  if(definitions.size() > 0) {
    std::cout << indent << additional << "Definitions:\n";
    for(DefinitionBox d : definitions) {
      d.print(indentation + 2, indentSize);
    }
  } else {
    std::cout << indent << additional << "Definitions: None\n";
  }
}
