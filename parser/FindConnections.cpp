#include<string>
#include<vector>
#include<iostream>

#include"Connection.hpp"
#include"FindConnections.hpp"

FindConnections::FindConnections(std::vector<std::string> c, std::vector<Pin> p) {
  contents.swap(c);
  pins.swap(p);
}

void FindConnections::findPath(std::size_t index) {
  int maxIterations = contents.size() * contents[0].size();
  int x = pins[index].x();
  int y = pins[index].y();
  Pin::DIRECTION direction = pins[index].getNormal();

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
    for(std::size_t i = 0; i < pins.size(); i++) {
      if(pins[i].x() == x)
        std::cout << "(MATCH, ";
      else
        std::cout << "(   " << pins[i].x() << ", ";

      if(pins[i].y() == y)
        std::cout << "MATCH)";
      else
        std::cout << "   " << pins[i].y() << ")";

      std::cout << std::endl;

      if(pins[i].x() == x && pins[i].y() == y) {
        connections.push_back(Connection(index, i, typeInfo, ordered));
        return;
      }
    }

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

    printMap(x, y);
  }
}

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

bool FindConnections::connected(size_t index) {
  for(Connection c : connections) {
    if(c.p1 == index || c.p2 == index) {
      return true;
    }
  }
  return false;
}

// for each pin
// check if it has already had it's connections made
// then follow the path
// note that this implementation does not allow the
// use of 't' or any character as a junction
void FindConnections::process() {
  for(std::size_t i = 0; i < pins.size(); i++) {
    // check if it already has a connection
    if(connected(i)) {
      continue;
    }

    // otherwise start traversing the line
    findPath(i);
  }
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

void FindConnections::move(std::vector<std::string> lines, std::vector<Pin> p, std::vector<Connection> cns) {
  contents.swap(lines);
  pins.swap(p);
  connections.swap(cns);
}
