#include<iostream>
#include<fstream>
#include<string>
#include<vector>

enum class Type {
  INT,
  CHAR,
  STRING,
  CUSTOM,
  UNKOWN
};

// Will have a char to identify it
// and store type information
// including whether the type is known
// for now, pins that have more that one type
// can just be represented as different pins
// located at the same location.
class Pin {
  public:
    Pin(char id, int x, int y, Type t = Type::UNKOWN) {
      identifier = id;
      posX = x;
      posY = y;
      type = t;
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
    Type getType() {
      return type;
    }
    std::string getTypeName() {
      return typeName;
    }
    void setType(Type t, std::string tn = "") {
      type = t;
      typeName = tn;
    }
  private:
    char identifier;
    int posX, posY;
    Type type;
    std::string typeName;
};

// path of a box seems to be
// get bounds, and store contents ->
// parse edges to find if def or desc ->
// return as formed def or desc

// because of "figure it out" approach to types
// there will probably be a need to reference
// and modify functions
// so maybe a parsed desc box has a reference to
// the function.
// if so then definitions will need to be parsed first

// The unparsed box is for storing gathered
// information for access and manipulation.
// It does not need to be representative of
// how it will be run just what it is/contains.
// It will store the it's position within
// wherever it is defined.
// It will store the string data, including
// the edges. 
// It will store a list of pins for itself.
// They will need to be flexible because their
// type might or might not be known at time of
// creation.
// It will need to store the boxes within it.
// And how these boxes connect to others and itself.
class UnparsedBox {
  public:
    void print() {
    }
    char at(int x, int y) {
      return contents.at(y)[x];
    }
  private:
    // top left relative to parent box
    int tlx, tly;

    // width/height stored for easy access
    int width, height;

    // the string data of the box
    std::vector<std::string> contents;

    // the box's pins
    std::vector<Pin> pins;

    // all the boxes contained within this box
    std::vector<UnparsedBox> boxes;

    // definitions
    // functions
};

// has pins, and because a definition doesn't
// connect to anything, it should be
// unambiguous from the definition's
// contents the types.
// It makes sense to have a definition
// for each overload.
// The definition box will have a list of
// definitions inside it
// as well as a list of function boxes and
// what they connect to.
class DefinitionBox {
  public:
  private:
    // none of these should have the UNKOWN type
    std::vector<Pin> pins;
    std::vector<DefinitionBox> definitions;
    std::vector<DescriptionBox> descriptions;
}

class DescriptionBox {
  public:
    std::string name;
    std::vector<Pin> pins;
}
class BreakDown {
  public:
    BreakDown(Box b);
    void print();
  private:
    Box b;

    // stores nested definitions within
    // the current box
    std::vector<Box> definitions;

    // the name of the current definition
    std::string name;

    std::vector<unclaimedIO> io;

    void parseEdges();
};

void BreakDown::parseEdges() {
  // check the corners are '+'
  if(b.at(b.topLeft()) != '+' || b.at(b.topRight()) != '+'
      || b.at(b.bottomLeft()) != '+' || b.at(b.bottomRight()) != '+') {
    // error
    return;
  }
  
}

// have a box
// which is just a list of strings
// From the strings you can find from
// the edges, 
// the name
// i/o pins
// an i/o pin can be stored as a position, and a char
// maybe also give a type if it is specified
class UnclaimedIO {
  public:
    UnclaimedIo(char pin, int x, int y, std::string type = "unspecified") {
      c = pin;
      xpos = x;
      ypos = y;
      t = type;
    }
    char getPin() {
      return c;
    }
    int getX() {
      return x;
    }
    int getY() {
      return y;
    }
  private:
    char c;
    // x and y relative to
    // 0, 0 the top left corner
    int xpos, ypos;
    // this might be better stored as an enum but I'll deal with that later
    std::string t;
};


int main() {
  Box b(0, 23, 1);
  b.add("+-------- main --------+");
  b.add("|    string literal    |");
  b.add("|  +----------------+  |");
  b.add("|  | \"Hello World!\" |  |");
  b.add("|  +-------o--------+  |");
  b.add("|          |           |");
  b.add("|      +---i---+       |");
  b.add("|      | print |       |");
  b.add("|      +-------+       |");
  b.add("|                      |");
  b.add("+----------------------+");

  BreakDown bd = BreakDown(b);
  bd.print();
}
