#include<iostream>
#include<fstream>
#include<string>
#include<vector>

class BoxFinder {
  public:
    BoxFinder();
    void process(std::string line);
    void printBoxes();
  private:
    class Box {
      public:
        Box(std::size_t start, std::size_t end, std::size_t lineNo) {
          s = start;
          e = end;
          l = lineNo;
        }
        std::size_t start() {
          return s;
        }
        std::size_t end() {
          return e;
        }
        void add(std::string row) {
          contents.push_back(row);
        }
        void print() {
          std::cout << s << " " << e << " " << l << std::endl;
          for(std::string s : contents) {
            std::cout << s << std::endl;
          }
        }
        Box move() {
          Box b = Box(s, e, l);
          contents.swap(b.contents);
          return b;
        }
      private:
        // how far the vertical edges of the box are from
        // the left edge of the file
        // and the line at which the box starts
        std::size_t s, e, l;
        std::vector<std::string> contents;
    };

    std::vector<Box> inProgress;
    std::vector<Box> finalised;
    std::size_t lineNo;

    void finaliseBox();
    void finaliseBox(std::size_t boxIndex);
    void addLine(std::size_t boxIndex, std::string line);
};

BoxFinder::BoxFinder() {
  lineNo = 0;
}

void BoxFinder::finaliseBox(std::size_t boxIndex) {
  // there's probably a better way of moving the contents
  // but I don't know what it is
  finalised.push_back(inProgress[boxIndex].move());
  inProgress.erase(inProgress.begin() + boxIndex);
}

void BoxFinder::addLine(std::size_t boxIndex, std::string line) {
  // the + 1 is because substring is exclusive of the last character
  std::size_t length = inProgress[boxIndex].end() - inProgress[boxIndex].start() + 1;
  std::string boxSegment = line.substr(inProgress[boxIndex].start(), length);
  inProgress[boxIndex].add(boxSegment);
}

void BoxFinder::process(std::string line) {
  // update line number
  // this also means that line number is 1 indexed
  lineNo++;

  // update all of the current boxes
  // and strike out all of the contents
  // so that they aren't picked up in the next section
  for(std::size_t i = 0; i < inProgress.size(); i++) {
    // storing start and end for strikeout
    std::size_t start = inProgress[i].start();
    std::size_t end = inProgress[i].end();

    // adds the appropriate line
    addLine(i, line);

    // checks if it should be the end of the box
    if(line[inProgress[i].start()] == '+') {
      finaliseBox(i);
      i--; // because you're removing an item from the vector
    }

    // strike out code
    // '.' is an arbitrary choice,
    // anything that isn't '+' should work
    for(std::size_t j = start; j <= end; j++) {
      line[j] = '.';
    }
  }

  // this is an odd use of the for loop
  // mostly here because I wondered if it was possible
  for(std::size_t search = line.find('+'); search != std::string::npos; search = line.find('+', search + 1)) {
    // TODO: I'm not even using search
    // this needs to be refactored
    std::size_t start = line.find('+');
    if(start == std::string::npos)
      return;

    std::size_t end = line.find('+', start + 1);
    if(end == std::string::npos)
      return; // this is prime candidate for error checking

    inProgress.push_back(Box(start, end, lineNo));
    addLine(inProgress.size() - 1, line);

    // what actually makes it work
    for(std::size_t j = start; j <= end; j++) {
      line[j] = '.';
    }
  }
}

void BoxFinder::printBoxes() {
  for(Box b : finalised) {
    b.print();
    std::cout << "\n";
  }
  // this should throw an error
  // something along the lines of "unclosed box at end of file"
  // actually this check shouldn't be in the print function
  std::cout << "Number of unfinalised: " << inProgress.size();
  std::cout << std::endl;
}

void findFunctions(std::ifstream &input) {
  BoxFinder boxFinder;
  std::string line;
  while(getline(input, line)) {
    boxFinder.process(line);
  }

  boxFinder.printBoxes();
}

int main(int argc, char* argv[]) {
  if(argc < 2) {
    std::cout << "Usage: ./test input1.box input2.box ..." << std::endl;
    return 0;
  }

  for(int i = 1; i < argc; i++) {
    // file not found exception?
    // or just check possible exceptions
    std::ifstream input(argv[i]);
    if(input.good()) {
      findFunctions(input);
    }
    input.close();
  }
}
