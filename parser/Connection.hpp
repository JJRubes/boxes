#pragma once

#include<string>

class Connection {
  public:
    Connection(size_t i, size_t j, std::string t, bool o) {
      p1 = i;
      p2 = j;
      type = t;
      ordered = o;
    }

    size_t p1, p2;
    bool ordered;
    std::string type;
};
