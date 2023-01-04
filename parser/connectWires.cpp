#include<string>
#include<vector>
#include<iostream>

#include"FindConnections.hpp"
#include"Pin.hpp"

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
    if(p.getId() != lines[p.y()][p.x()]) {
      std::cout << "Pin " << p.getId() << " at (" << p.x() << ", " << p.y() << ") doesn't exist in text" << std::endl;
    }
  }

 FindConnections fc = FindConnections(lines, pins);
 fc.process();
 fc.print();
}
