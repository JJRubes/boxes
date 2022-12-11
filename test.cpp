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
  std::cout << "updating box: " << std::endl;
  inProgress[boxIndex].print();
  std::cout << "substring of: " << line << std::endl;
  std::cout << "from: " << inProgress[boxIndex].start() << ", to: " << inProgress[boxIndex].end() << std::endl;
  // the + 1 is because substring is exclusive of the last character
  std::size_t length = inProgress[boxIndex].end() - inProgress[boxIndex].start() + 1;
  std::string boxSegment = line.substr(inProgress[boxIndex].start(), length);
  inProgress[boxIndex].add(boxSegment);
  std::cout << "passed successfully" << std::endl;
  std::cout << "resulting box: " << std::endl;
  inProgress[boxIndex].print();
}

void BoxFinder::process(std::string line) {
  for(std::size_t i = 0; i < inProgress.size(); i++) {
    std::size_t start = inProgress[i].start();
    std::size_t end = inProgress[i].end();
    addLine(i, line);
    if(line[inProgress[i].start()] == '+') {
      finaliseBox(i);
      i--; // because you're removing an item from the vector
    }
    for(std::size_t j = start; j <= end; j++) {
      line[j] = '.';
    }
  }

  for(std::size_t search = line.find('+'); search != std::string::npos; search = line.find('+', search + 1)) {
    std::size_t start = line.find('+');
    if(start == std::string::npos)
      return;

    std::size_t end = line.find('+', start + 1);
    if(end == std::string::npos)
      return;

    inProgress.push_back(Box(start, end));
    addLine(inProgress.size() - 1, line);

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
