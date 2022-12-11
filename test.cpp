#include<iostream>
#include<fstream>
#include<string>
#include<vector>

class BoxFinder {
  public:
    void process(std::string line);
    void printBoxes();
  private:
    class Box {
      public:
        Box(std::size_t start, std::size_t end) {
          s = start;
          e = end;
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
          for(std::string s : contents) {
            std::cout << s << std::endl;
          }
        }
        Box move() {
          Box b = Box(s, e);
          contents.swap(b.contents);
          return b;
        }
      private:
        // how far the vertical edges of the box are from
        // the left edge of the file
        std::size_t s, e;
        std::vector<std::string> contents;
    };

    std::vector<Box> inProgress;
    std::vector<Box> finalised;

    void finaliseBox();
    void finaliseBox(std::size_t boxIndex);
    void addLine(std::size_t boxIndex, std::string line);
};

void BoxFinder::finaliseBox(std::size_t boxIndex) {
  finalised.push_back(inProgress[boxIndex].move());
  inProgress.erase(inProgress.begin() + boxIndex);
}

void BoxFinder::addLine(std::size_t boxIndex, std::string line) {
  std::string boxSegment = line.substr(inProgress[0].start(), inProgress[0].end());
  inProgress[0].add(boxSegment);
}

void BoxFinder::process(std::string line) {
  if(inProgress.size() < 1) {
    std::size_t start = line.find('+');
    if(start == std::string::npos)
      return;

    std::size_t end = line.find('+', start + 1);
    if(end == std::string::npos)
      return;

    inProgress.push_back(Box(start, end + 1));
    addLine(0, line);
  } else {

    for(std::size_t i = 0; i < inProgress.size(); i++) {
      addLine(i, line);
      if(line[inProgress[i].start()] == '+') {
        finaliseBox(i);
      }
    }
  }
}

void BoxFinder::printBoxes() {
  for(Box b : finalised) {
    b.print();
    std::cout << "\n";
  }
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
  if(argc != 2) {
    std::cout << "Usage: ./test input.box" << std::endl;
    return 0;
  }

  std::ifstream input(argv[1]);
  if(input.good()) {
    findFunctions(input);
  }
  input.close();
}
