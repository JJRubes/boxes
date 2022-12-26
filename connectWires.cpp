#include<string>
#include<vector>
#include<iostream>

class Pin {
  public:
    enum class DIRECTION {
      NORTH,
      SOUTH,
      EAST,
      WEST
    };

    Pin(char iden, int posX, int posY, DIRECTION n) {
      id = iden;
      x = posX;
      y = posY;
      normal = n;
    }

    int x, y;
    char id;
    DIRECTION normal;
};

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

class FindConnections {
  public:
    FindConnections(std::vector<std::string> c, std::vector<Pin> p);
    void process();
    void print();

  private:
    std::vector<std::string> contents;
    std::vector<Pin> pins;
    std::vector<Connection> connections;

    bool connected(std::size_t index);
    void findPath(std::size_t index);
    void printMap(int x, int y);
};

FindConnections::FindConnections(std::vector<std::string> c, std::vector<Pin> p) {
  contents.swap(c);
  pins.swap(p);
}

void FindConnections::findPath(std::size_t index) {
  int maxIterations = contents.size() * contents[0].size();
  int x = pins[index].x;
  int y = pins[index].y;
  Pin::DIRECTION direction = pins[index].normal;

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
      if(pins[i].x == x)
        std::cout << "(MATCH, ";
      else
        std::cout << "(   " << pins[i].x << ", ";

      if(pins[i].y == y)
        std::cout << "MATCH)";
      else
        std::cout << "   " << pins[i].y << ")";

      std::cout << std::endl;

      if(pins[i].x == x && pins[i].y == y) {
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
    if(i == y) {
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

int main() {
  std::vector<std::string> lines;
  //               0         1         2         3
  //               012345678901234567890123456789012
  lines.push_back("+---------- parse op -----------+"); // 0
  lines.push_back("|                               |"); // 1 
  lines.push_back("|      +---+ +---+ +---+ +---+  |"); // 2 
  lines.push_back("|      |'+'| |'-'| |'*'| |'/'|  |"); // 3 
  lines.push_back("|      +-o-+ +-o-+ +-o-+ +-o-+  |"); // 4 
  lines.push_back("|        |     |     |     |    |"); // 5 
  lines.push_back("|      +-x-----y-----z-----w-+  |"); // 6 
  lines.push_back(">-char-i case                o--o"); // 7 
  lines.push_back("|      +-a-----b-----c-----d-+  |"); // 8 
  lines.push_back("|        |     |     |     |    |"); // 9 
  lines.push_back("|      +-o-+ +-o-+ +-o-+ +-o-+  |"); // 10 
  lines.push_back("|      |x+y| |x-y| |x*y| |x/y|  |"); // 11 
  lines.push_back("|      |+o+| |+o+| |+o+| |+o+|  |"); // 12 
  lines.push_back("|      +---+ +---+ +---+ +---+  |"); // 13 
  lines.push_back("|                               |"); // 14 
  lines.push_back("+-------------------------------+"); // 15 
  std::vector<Pin> pins;
  pins.push_back(Pin('o',  9,  4, Pin::DIRECTION::SOUTH));
  pins.push_back(Pin('o', 15,  4, Pin::DIRECTION::SOUTH));
  pins.push_back(Pin('o', 21,  4, Pin::DIRECTION::SOUTH));
  pins.push_back(Pin('o', 27,  4, Pin::DIRECTION::SOUTH));
  pins.push_back(Pin('x',  9,  6, Pin::DIRECTION::NORTH));
  pins.push_back(Pin('y', 15,  6, Pin::DIRECTION::NORTH));
  pins.push_back(Pin('z', 21,  6, Pin::DIRECTION::NORTH));
  pins.push_back(Pin('w', 27,  6, Pin::DIRECTION::NORTH));
  pins.push_back(Pin('>',  0,  7, Pin::DIRECTION::EAST));
  pins.push_back(Pin('i',  7,  7, Pin::DIRECTION::WEST));
  pins.push_back(Pin('o', 29,  7, Pin::DIRECTION::EAST));
  pins.push_back(Pin('o', 32,  7, Pin::DIRECTION::WEST));
  pins.push_back(Pin('a',  9,  8, Pin::DIRECTION::SOUTH));
  pins.push_back(Pin('b', 15,  8, Pin::DIRECTION::SOUTH));
  pins.push_back(Pin('c', 21,  8, Pin::DIRECTION::SOUTH));
  pins.push_back(Pin('d', 27,  8, Pin::DIRECTION::SOUTH));
  pins.push_back(Pin('o',  9, 10, Pin::DIRECTION::NORTH));
  pins.push_back(Pin('o', 15, 10, Pin::DIRECTION::NORTH));
  pins.push_back(Pin('o', 21, 10, Pin::DIRECTION::NORTH));
  pins.push_back(Pin('o', 27, 10, Pin::DIRECTION::NORTH));

  // first check that everything is where it says it is
  for(Pin p : pins) {
    if(p.id != lines[p.y][p.x]) {
      std::cout << "Pin " << p.id << " at (" << p.x << ", " << p.y << ") doesn't exist in text" << std::endl;
    }
  }

 FindConnections fc = FindConnections(lines, pins);
 fc.process();
 fc.print();
}
