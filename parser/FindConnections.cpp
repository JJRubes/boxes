#include<string>
#include<vector>
#include<iostream>

#include"CallBox.hpp"
#include"Connection.hpp"
#include"DefinitionBox.hpp"
#include"FindConnections.hpp"
#include"Pin.hpp"

// references to vectors are used here (instead of swaps) as
// the class is not the owner, it just needs access
FindConnections::FindConnections(const std::vector<std::string> &c, 
                const std::vector<Pin> &p,
                const std::vector<CallBox> &cb,
                const std::vector<DefinitionBox> &db,
                int x, int y) {
  contents = c;
  pins = p;
  definitions = db;
  calls = cb;

  selfOffsetX = x;
  selfOffsetY = y;
}

void FindConnections::findPath(Connection::BoxType t, std::size_t b, std::size_t p) {
  int maxIterations = contents.size() * contents[0].size();
  int x = getPinX(t, b, p);
  int y = getPinY(t, b, p);
  Pin::DIRECTION direction = getPin(t, b, p).getNormal();

  // if(t == Connection::BoxType::SELF) {
  //   std::cout << "Finding path from self." << getPin(t, b, p).getId() << std::endl;
  // } else if(t == Connection::BoxType::CALL) {
  //   std::cout << "Finding path from call \"" << calls[b].getName() << "\", '" << getPin(t, b, p).getId() << "'" << std::endl;
  // } else {
  //   std::cout << "Finding path from definition \"" << definitions[b].getName() << "\", '" << getPin(t, b, p).getId() << "'" << std::endl;
  // }
  // std::cout << "Starting at (" << x << ", " << y << ")" << std::endl;

  std::string typeInfo = "";
  bool ordered = false;

  int iterations = 0;
  while(true) {
    // update the current position
    switch(direction) {
      case Pin::DIRECTION::NORTH:
        y--;
        break;
      case Pin::DIRECTION::SOUTH:
        y++;
        break;
      case Pin::DIRECTION::EAST:
        x++;
        break;
      case Pin::DIRECTION::WEST:
        x--;
        break;
    }

    // check for a pin here
    if(makeConnection(t, b, p, typeInfo, ordered, x, y))
      return;

    // otherwise check the current position
    // to see how to continue on the path
    char currentChar = contents[y][x];
    switch(currentChar) {
      case '^':
        if(direction == Pin::DIRECTION::SOUTH) {
          // if it is ordered then that means that there are
          // conflicting arrows
          // e.g.
          // -->---<--
          if(ordered) {
            // error
            std::cout << "Error: Conflicting direction markings" << std::endl;
          }

          // this means that this pin is an input pin
          // and that we shouldn't be following the path this way
          return;
        }
        direction = Pin::DIRECTION::NORTH;
        ordered = true;
        break;
      case 'v':
        if(direction == Pin::DIRECTION::NORTH) {
          if(ordered) {
            // error
            std::cout << "Error: Conflicting direction markings" << std::endl;
          }

          // this means that this pin is an input pin
          // and that we shouldn't be following the path this way
          return;
        }
        direction = Pin::DIRECTION::SOUTH;
        ordered = true;
        break;
      case '>':
        if(direction == Pin::DIRECTION::WEST) {
          if(ordered) {
            // error
            std::cout << "Error: Conflicting direction markings" << std::endl;
          }

          // this means that this pin is an input pin
          // and that we shouldn't be following the path this way
        }
        direction = Pin::DIRECTION::EAST;
        ordered = true;
        break;
      case '<':
        if(direction == Pin::DIRECTION::EAST) {
          if(ordered) {
            // error
            std::cout << "Error: Conflicting direction markings" << std::endl;
          }

          // this means that this pin is an input pin
          // and that we shouldn't be following the path this way
        }
        direction = Pin::DIRECTION::WEST;
        ordered = true;
        break;
      case '|':
        if(direction == Pin::DIRECTION::EAST || direction == Pin::DIRECTION::WEST) {
          // this means you've crossed a path that you shouldn't
          std::cout << "Error: Abrupt change in path direction" << std::endl;
        }
        break;
      case '-':
        if(direction == Pin::DIRECTION::NORTH || direction == Pin::DIRECTION::SOUTH) {
          // this means you've crossed a path that you shouldn't
          std::cout << "Error: Abrupt change in path direction" << std::endl;
        }
        break;
      case 'x':
        // this is a crossroad, nothing needs to be done
        break;
      default:
        typeInfo += currentChar;
        break;
    }

    if(iterations > maxIterations) {
      std::cout << "infinite loop" << std::endl;
      return;
    }
    iterations++;

    // printMap(x, y);
  }
}

// debugging info that shows the whole box and where it is looking
void FindConnections::printMap(int x, int y) {
  std::cout << "Current character: " << contents[y][x] << std::endl;
  for(std::size_t i = 0; i < contents.size(); i++) {
    if(i == (size_t)y) {
      std::cout << contents[i].substr(0, x);
      std::cout << "*";
      std::cout << contents[i].substr(x + 1, std::string::npos);
    } else {
      std::cout << contents[i];
    }
    std::cout << std::endl;
  }
}

// checks if a pin has already been connected
bool FindConnections::connected(Connection::BoxType t, size_t b, size_t p) {
  for(Connection c : connections) {
    if((c.t1 == t && c.b1 == b && c.p1 == p) || (c.t2 == t && c.b2 == b && c.p2 == p)) {
      return true;
    }
  }
  return false;
}

Pin FindConnections::getPin(Connection::BoxType t, size_t b, size_t p) {
  switch(t) {
    case Connection::BoxType::SELF:
      return pins[p];
    case Connection::BoxType::CALL:
      return calls[b].getPin(p);
    case Connection::BoxType::DEFINITION:
      return definitions[b].getPin(p);
    default:
      std::cout << "Error" << std::endl;
      return Pin('x', 0, 0, Pin::DIRECTION::NORTH);
  }
}

bool FindConnections::makeConnection(Connection::BoxType t, size_t b, size_t p, std::string typeInfo, bool ordered, int x, int y) {
  // first check self
  for(std::size_t i = 0; i < pins.size(); i++) {
    int currx = getPinX(Connection::BoxType::SELF, 0, i);
    int curry = getPinY(Connection::BoxType::SELF, 0, i);

    // if(currx == x)
    //   std::cout << "(MATCH, ";
    // else
    //   std::cout << "(   " << currx << ", ";

    // if(curry == y)
    //   std::cout << "MATCH)";
    // else
    //   std::cout << "   " << curry << ")";

    // std::cout << std::endl;

    if(currx == x && curry == y) {
      Connection c(t, b, p,
                   Connection::BoxType::SELF, 0, i,
                   typeInfo, ordered);
      connections.push_back(c);
      return true;
    }
  }

  // then calls
  for(std::size_t i = 0; i < calls.size(); i++) {
    for(std::size_t j = 0; j < calls[i].getPins().size(); j++) {
      int currx = getPinX(Connection::BoxType::CALL, i, j);
      int curry = getPinY(Connection::BoxType::CALL, i, j);

      // if(currx == x)
      //   std::cout << "(MATCH, ";
      // else
      //   std::cout << "(   " << currx << ", ";

      // if(curry == y)
      //   std::cout << "MATCH)";
      // else
      //   std::cout << "   " << curry << ")";

      // std::cout << std::endl;

      if(currx == x && curry == y) {
        Connection c(t, b, p,
                     Connection::BoxType::CALL, i, j,
                     typeInfo, ordered);
        connections.push_back(c);
        return true;
      }
    }
  }

  // definition pins don't need to be connected to anything outside themselves
  // for(std::size_t i = 0; i < definitions.size(); i++) {
  //   for(std::size_t j = 0; j < definitions[i].getPins().size(); j++) {
  //     int currx = getPinX(Connection::BoxType::DEFINITION, i, j);
  //     int curry = getPinY(Connection::BoxType::DEFINITION, i, j);
  //     if(currx == x)
  //       std::cout << "(MATCH, ";
  //     else
  //       std::cout << "(   " << currx << ", ";

  //     if(curry == y)
  //       std::cout << "MATCH)";
  //     else
  //       std::cout << "   " << curry << ")";

  //     std::cout << std::endl;
  //     if(currx == x && curry == y) {
  //       Connection c(t, b, p,
  //                    Connection::BoxType::DEFINITION, i, j,
  //                    typeInfo, ordered);
  //       connections.push_back(c);
  //       return true;
  //     }
  //   }
  // }

  return false;
}

// for each pin
// check if it has already had it's connections made
// then follow the path
// note that this implementation does not allow the
// use of 't' or any character as a junction
void FindConnections::process() {
  // Self
  for(std::size_t i = 0; i < pins.size(); i++) {
    // check if it already has a connection
    if(connected(Connection::BoxType::SELF, 0, i)) {
      continue;
    }

    // otherwise start traversing the line
    findPath(Connection::BoxType::SELF, 0, i);
  }

  // then calls
  for(std::size_t i = 0; i < calls.size(); i++) {
    for(std::size_t j = 0; j < calls[i].getPins().size(); j++) {
      // check if it already has a connection
      if(connected(Connection::BoxType::CALL, i, j)) {
        continue;
      }

      // otherwise start traversing the line
      findPath(Connection::BoxType::CALL, i, j);
    }
  }

  // then definitions
  // for(std::size_t i = 0; i < definitions.size(); i++) {
  //   for(std::size_t j = 0; j < definitions[i].getPins().size(); j++) {
  //     // check if it already has a connection
  //     if(connected(Connection::BoxType::DEFINITION, i, j)) {
  //       continue;
  //     }

  //     // otherwise start traversing the line
  //     findPath(Connection::BoxType::DEFINITION, i, j);
  //   }
  // }
}

int FindConnections::getPinX(Connection::BoxType t, size_t b, size_t p) {
  int x = getPin(t, b, p).x();
  if(t == Connection::BoxType::SELF) {
    // x -= selfOffsetX;
  } else if(t == Connection::BoxType::CALL) {
    x += calls[b].getX();
  } else {
    x += definitions[b].getX();
  }
  return x;
}

int FindConnections::getPinY(Connection::BoxType t, size_t b, size_t p) {
  int y = getPin(t, b, p).y();
  if(t == Connection::BoxType::SELF) {
    // y -= selfOffsetY;
  } else if(t == Connection::BoxType::CALL) {
    y += calls[b].getY();
  } else {
    y += definitions[b].getY();
  }
  return y;
}

void FindConnections::print() {
  for(Connection c : connections) {
    std::cout << "P1: " << c.p1;
    std::cout << ", P2: " << c.p2;
    std::cout << ", type: " << c.type;
    std::cout << ", Ordered: " << c.ordered;
    std::cout << std::endl;
  }
}
