#include<string>
#include<vector>

#include"BoxFinder.hpp"
#include"CallBox.hpp"
#include"DefinitionBox.hpp"
#include"FindConnections.hpp"
#include"Pin.hpp"
#include"UnparsedBox.hpp"

UnparsedBox::UnparsedBox(int x, int y, std::vector<std::string> lines) {
  tlx = x;
  tly = y;
  contents.swap(lines);

  // this assumes every line is consistent in length
  width = contents[0].size();
  height = contents.size();

  edgesParsed = false;
  definition = false;
}

bool UnparsedBox::isDef() {
  if(!edgesParsed)
    std::cout << "error" << std::endl;

  return definition;
}

void UnparsedBox::parseEdges() {
  if(edgesParsed)
    std::cout << "error" << std::endl;

  parseName();

  // parse the pins
  // top
  for(int i = 0; i < width; i++) {
    if(isPin(at(i, 0))) {
      if(isDef())
        pins.push_back(Pin(at(i, 0), i, 0, Pin::DIRECTION::NORTH));
      else
        pins.push_back(Pin(at(i, 0), i, 0, Pin::DIRECTION::SOUTH));
    }
  }

  // left
  for(int i = 0; i < height; i++) {
    if(isPin(at(0, i))) {
      if(isDef())
        pins.push_back(Pin(at(0, i), 0, i, Pin::DIRECTION::WEST));
      else
        pins.push_back(Pin(at(0, i), 0, i, Pin::DIRECTION::EAST));
    }
  }

  // right
  for(int i = 0; i < height; i++) {
    if(isPin(at(width - 1, i))) {
      if(isDef())
        pins.push_back(Pin(at(width - 1, i), width - 1, i, Pin::DIRECTION::EAST));
      else
        pins.push_back(Pin(at(width - 1, i), width - 1, i, Pin::DIRECTION::WEST));
    }
  }

  // bottom
  for(int i = 0; i < width; i++) {
    if(isPin(at(i, height - 1))) {
      if(isDef())
        pins.push_back(Pin(at(i, height - 1), i, height - 1, Pin::DIRECTION::SOUTH));
      else
        pins.push_back(Pin(at(i, height - 1), i, height - 1, Pin::DIRECTION::NORTH));
    }
  }
}

DefinitionBox UnparsedBox::makeDefinition() {
  if(!edgesParsed)
    std::cout << "error" << std::endl;

  if(!definition)
    std::cout << "error" << std::endl;

  std::vector<DefinitionBox> definitions;
  std::vector<CallBox> calls;

  BoxFinder bf;
  for(std::size_t i = 1; i < contents.size() - 1; i++) {
    std::size_t len = contents[i].size();
    bf.process(contents[i].substr(1, len - 2));
  }

  std::vector<UnparsedBox> boxes;
  bf.move(boxes);

  for(UnparsedBox b : boxes) {
    b.parseEdges();
    if(b.isDef()) {
      definitions.push_back(b.makeDefinition());
    } else {
      calls.push_back(b.makeCall());
    }
  }

  std::vector<Connection> connections;
  // this does a swap on contents and pins
  FindConnections fc = FindConnections(contents, pins);
  fc.process();
  // very hacky way around just passing the vectors by reference
  fc.move(contents, pins, connections);

  return DefinitionBox(name, pins, definitions, calls, connections);
}

CallBox UnparsedBox::makeCall() {
  if(!edgesParsed)
    std::cout << "error" << std::endl;

  if(definition)
    std::cout << "error" << std::endl;

  name = "";
  for(std::size_t i = 1; i < contents.size() - 1; i++) {
    std::string l = "";
    bool maybeEnd = false;
    int spaceCount;
    for(char c : contents.at(i)) {
      if(maybeEnd) {
        if(c == ' ') {
          spaceCount++;
        } else {
          maybeEnd = false;
          std::string s(spaceCount, ' ');
          l += s;
          l += c;
        }
      } else {
        if(c == ' ') {
          maybeEnd = true;
          spaceCount = 1;
        } else {
          l += c;
        }
      }
    }
    if(l != "") {
      name += " " + l;
    }
  }
  return CallBox(name, pins);
}

void UnparsedBox::print() {
  if(!edgesParsed) {
    for(std::string s : contents) {
      std::cout << s << std::endl;
    }
  } else if(isDef()) {
    std::cout << name << ": " << std::endl;
    for(Pin p : pins) {
      p.print();
    }
  }
}

const bool UnparsedBox::isPin(char p) {
  return p != '+' &&
         p != '-' &&
         p != '|' &&
         p != ' ';
}

char UnparsedBox::at(int x, int y) {
  return contents.at(y)[x];
}

void UnparsedBox::parseName() {
  edgesParsed = true;

  // check the top line
  int start = scanHorizontal(0, true);
  if(start != -1) {
    std::cout << "found start of name at " << start << std::endl;
    int end = scanHorizontal(0, false);
    std::cout << "found end of name at " << end << std::endl;
    if(end < start) {
      std::cout << "Error: found spaces on top edge that didn't contain a name." << std::endl;
      return;
    }
    getName(start, end, 0, true);
    return;
  }

  // left edge
  start = scanVertical(0, true);
  if(start != -1) {
    int end = scanVertical(0, false);
    if(end < start) {
      std::cout << "Error: found spaces on left edge that didn't contain a name." << std::endl;
      return;
    }
    getName(start, end, 0, false);
    return;
  }

  // right edge
  start = scanVertical(width - 1, true);
  if(start != -1) {
    int end = scanVertical(width - 1, false);
    if(end < start) {
      std::cout << "Error: found spaces on right edge that didn't contain a name." << std::endl;
      return;
    }
    getName(start, end, width - 1, false);
    return;
  }

  // bottom edge
  start = scanHorizontal(height - 1, true);
  if(start != -1) {
    int end = scanHorizontal(height - 1, false);
    if(end < start) {
      std::cout << "Error: found spaces on bottom edge that didn't contain a name." << std::endl;
      return;
    }
    getName(start, end, height - 1, true);
    return;
  }
}

int UnparsedBox::scanHorizontal(int line, bool forwards) {
  std::cout << "scanning line " << line << std::endl;
  bool spaces = false;
  int last = -1;
  int start = forwards ? 0 : width - 1;
  int end = forwards ? width : -1;
  int inc = forwards ? 1 : -1;
  for(int i = start; inc * i < end; i += inc) {
    std::cout << at(i, line) << ", " << i << std::endl;
    if(spaces) {
      if(at(i, line) != ' ') {
        last = i;
        break;
      }
    } else {
      if(at(i, line) == ' ') {
        spaces = true;
      }
    }
  }
  return last;
}

int UnparsedBox::scanVertical(int line, bool forwards) {
  bool spaces = false;
  int last = -1;
  int start = forwards ? 0 : height - 1;
  int end = forwards ? height : -1;
  int inc = forwards ? 1 : -1;
  for(int i = start; i < end; i += inc) {
    if(spaces) {
      if(at(line, i) != ' ') {
        last = i;
        break;
      }
    } else {
      if(at(line, i) == ' ') {
        spaces = true;
      }
    }
  }
  return last;
}

void UnparsedBox::getName(int start, int end, int line, bool horizontal) {
  if(horizontal) {
    name = std::string(contents.at(line), start, end - start + 1);
    std::cout << "name is " << name << std::endl;
    // then blank it
    for(int i = start; i <= end; i++) {
      contents[line][i] = '-';
    }
  } else {
    name = std::string(start - end + 1, '.');
    for(int i = start; i <= end; i++) {
      name[i] = at(line, i);
    }
    // blank it
    for(int i = start; i <= end; i++) {
      contents[i][line] = '|';
    }
  }
  definition = true;
}
