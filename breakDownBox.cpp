#include<iostream>
#include<fstream>
#include<string>
#include<vector>

class Pin {
  public:
    Pin(char id, int x, int y) {
      identifier = id;
      posX = x;
      posY = y;
    }
    char getId() {
      return identifier;
    }
    int x() {
      return posX;
    }
    int y() {
      return posY;
    }
    void print() {
      std::cout << "id: " << identifier << std::endl;
      std::cout << "pos: (" << posX << ", " << posY << ")" << std::endl;
    }
  private:
    char identifier;
    int posX, posY;
};

class CallBox {
  public:
    CallBox(std::string n, std::vector<Pin> p) {
      name = n;
      pins.swap(p);
    }

    void print() {
      std::cout << "Call:" << std::endl;
      std::cout << "name: " << name << std::endl;
      std::cout << "pins:" << std::endl;
      for(Pin p : pins) {
        p.print();
      }
    }

  private:
    std::string name;
    std::vector<Pin> pins;
};

class DefinitionBox {
  public:
    DefinitionBox(std::string n, std::vector<Pin> p, std::vector<DefinitionBox> defs, std::vector<CallBox> cs) {
      name = n;
      pins.swap(p);
      definitions.swap(defs);
      calls.swap(cs);
    }
  private:
    std::string name;
    std::vector<Pin> pins;
    std::vector<DefinitionBox> definitions;
    std::vector<CallBox> calls;
};

class UnparsedBox {
  public:
    UnparsedBox(int x, int y, std::vector<std::string> lines) {
      tlx = x;
      tly = y;
      contents.swap(lines);

      // this assumes every line is consistent in length
      width = contents[0].size();
      height = contents.size();

      edgesParsed = false;
      definition = false;
    }

    bool isDef() {
      if(!edgesParsed)
        std::cout << "error" << std::endl;

      return definition;
    }

    void parseEdges() {
      if(edgesParsed)
        std::cout << "error" << std::endl;

      parseName();

      // parse the pins
      // top
      for(int i = 0; i < width; i++) {
        if(isPin(at(i, 0))) {
          pins.push_back(Pin(at(i, 0), i, 0));
        }
      }

      // left
      for(int i = 0; i < height; i++) {
        if(isPin(at(0, i))) {
          pins.push_back(Pin(at(0, i), 0, i));
        }
      }

      // right
      for(int i = 0; i < height; i++) {
        if(isPin(at(width - 1, i))) {
          pins.push_back(Pin(at(width - 1, i), width - 1, i));
        }
      }

      // bottom
      for(int i = 0; i < width; i++) {
        if(isPin(at(i, height - 1))) {
          pins.push_back(Pin(at(i, height - 1), i, height - 1));
        }
      }

      edgesParsed = true;
    }

    DefinitionBox makeDefinition() {
      if(!edgesParsed)
        std::cout << "error" << std::endl;

      if(!definition)
        std::cout << "error" << std::endl;

      std::vector<DefinitionBox> definitions;
      std::vector<CallBox> calls;

      // find boxes would be here
      // connect edges would be here

      return DefinitionBox(name, pins, definitions, calls);
    }

    CallBox makeCall() {
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

    void print() {
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

  private:
    const bool isPin(char p) {
      return p != '+' &&
             p != '-' &&
             p != '|' &&
             p != ' ';
    }

    char at(int x, int y) {
      return contents.at(y)[x];
    }

    void parseName() {
      // check the top line
      int start = scanHorizontal(0, true);
      if(start != -1) {
      std::cout << "here" << std::endl;
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

    int scanHorizontal(int line, bool forwards) {
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

    int scanVertical(int line, bool forwards) {
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

    void getName(int start, int end, int line, bool horizontal) {
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

    bool edgesParsed;
    // makes it easier to handle the
    // def/call distinction
    bool definition;

    std::string name;

    // top left relative to parent box
    int tlx, tly;

    // width/height stored for easy access
    int width, height;

    // the string data of the box
    std::vector<std::string> contents;

    // the box's pins
    std::vector<Pin> pins;
};

int main() {
  std::vector<std::string> lines;
  lines.push_back("+---------- parse op -----------+");
  lines.push_back("|                               |");
  lines.push_back("|      +---+ +---+ +---+ +---+  |");
  lines.push_back("|      |'+'| |'-'| |'*'| |'/'|  |");
  lines.push_back("|      +-o-+ +-o-+ +-o-+ +-o-+  |");
  lines.push_back("|        |     |     |     |    |");
  lines.push_back("|      +-x-----y-----z-----w-+  |");
  lines.push_back(">-char-i case                o--o");
  lines.push_back("|      +-a-----b-----c-----d-+  |");
  lines.push_back("|        |     |     |     |    |");
  lines.push_back("|      +-o-+ +-o-+ +-o-+ +-o-+  |");
  lines.push_back("|      |x+y| |x-y| |x*y| |x/y|  |");
  lines.push_back("|      |+o+| |+o+| |+o+| |+o+|  |");
  lines.push_back("|      +---+ +---+ +---+ +---+  |");
  lines.push_back("|                               |");
  lines.push_back("+-------------------------------+");

  UnparsedBox b(1, 1, lines);
  b.print();
  b.parseEdges();
  b.print();
  if(!b.isDef()) {
    CallBox cb = b.makeCall();
    cb.print();
  }
}
