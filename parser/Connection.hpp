#pragma once

#include<string>

class Connection {
  public:
    enum class BoxType {
      SELF,
      DEFINITION,
      CALL
    };

    Connection(BoxType type1, size_t box1, size_t pin1,
               BoxType type2, size_t box2, size_t pin2,
               std::string t, bool o) {
      t1 = type1;
      b1 = box1;
      p1 = pin1;
      t2 = type2;
      b2 = box2;
      p2 = pin2;
      type = t;
      ordered = o;
    }

    void print(std::size_t indentation, std::size_t indentSize) {
      std::string indent(indentation * indentSize, ' ');
      std::string additional(indentSize, ' ');
      std::cout << indent << "Connection:\n";
      std::cout << indent << additional << "From:\n";
      std::cout << indent << additional << additional << "Box: ";
      if(t1 == BoxType::SELF)
        std::cout << "Self\n";
      else if(t1 == BoxType::CALL) {
        std::cout << "Call[" << b1 << "]\n";
      } else {
        std::cout << "Definition[" << b1 << "]\n";
      }
      std::cout << indent << additional << additional << additional << "Pin: " << p1;
      std::cout << indent << additional << "To:\n";
      std::cout << indent << additional << additional << "Box: ";
      if(t2 == BoxType::SELF)
        std::cout << "Self\n";
      else if(t2 == BoxType::CALL) {
        std::cout << "Call[" << b2 << "]\n";
      } else {
        std::cout << "Definition[" << b2 << "]\n";
      }
      std::cout << indent << additional << additional << additional << "Pin: " << p2;
    }

    BoxType t1, t2;
    size_t b1, b2;
    size_t p1, p2;
    bool ordered;
    std::string type;
};
