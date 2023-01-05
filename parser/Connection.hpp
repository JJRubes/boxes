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

    BoxType t1, t2;
    size_t b1, b2;
    size_t p1, p2;
    bool ordered;
    std::string type;
};
