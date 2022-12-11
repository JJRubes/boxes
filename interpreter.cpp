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
          std::cout << "Adding row: " << row << std::endl;
          contents.push_back(row);
          for(int i = 0; i < contents.size(); i++) {
            std::cout << contents[i] << std::endl;
          }
        }
        void print() {
          std::cout << "Box height: " << contents.size() << std::endl;
          for(std::string s : contents) {
            std::cout << s << std::endl;
          }
        }
        std::vector<std::string> contents;
      private:
        // how far the vertical edges of the box are from
        // the left edge of the file
        std::size_t s, e;
    };

    std::vector<Box> inProgress;
    std::vector<Box> finalised;

    std::string getHedge(std::size_t pos, std::string line);
    std::string getHedge(std::size_t pos, std::string line, std::size_t length);
    int onCorner(std::size_t pos);
    bool contained(std::size_t pos);
    int finaliseBox(int index, std::string line);
    int createBox(std::size_t pos, std::string line);
};

int BoxFinder::onCorner(std::size_t pos) {
  for(std::size_t i = 0; i < inProgress.size(); i++) {
    if(pos == inProgress[i].start()) {
      return i;
    }
  }
  return -1;
}

std::string BoxFinder::getHedge(std::size_t pos, std::string line,
    std::size_t length) {
  std::string hedge = getHedge(pos, line);
  if(hedge.size() != length) {
    std::cout << "fuck" << std::endl;
  }
  return hedge;
}

std::string BoxFinder::getHedge(std::size_t pos, std::string line) {
  if(pos > line.size()) {
    std::cout << "Aaaah" << std::endl;
  }
  std::size_t end = line.find('+', pos + 1);
  if(end == std::string::npos) {
    std::cout << "Fuck" << std::endl;
  }
  return line.substr(pos, end - pos + 1);
}

int BoxFinder::finaliseBox(int index, std::string line) {
  int covered = inProgress[index].end();
  int length = inProgress[index].end() - inProgress[index].start();
  std::cout << "In finBox, pos is: " << inProgress[index].start() << std::endl;
  std::string hedge = getHedge(inProgress[index].start(), line, length);
  inProgress[index].add(hedge);
  finalised.push_back(Box(inProgress[index].start(), inProgress[index].end()));
  finalised[finalised.size() - 1].contents.swap(inProgress[index].contents);
  inProgress.erase(inProgress.begin() + index);
  return covered;
}

bool BoxFinder::contained(std::size_t pos) {
  for(Box b : inProgress) {
    if(pos > b.start() && pos < b.end())
      return true;
  }
  return false;
}

int BoxFinder::createBox(std::size_t pos, std::string line) {
  std::cout << "In createBox, pos is: " << pos << std::endl;
  std::string hedge = getHedge(pos, line);
  std::cout << "Create hedge: " << hedge << std::endl;
  Box b = Box(pos, pos + hedge.size());
  b.add(hedge);
  std::cout << "fresh box has start, " << b.start() << ", end: "<< b.end() << std::endl;
  inProgress.push_back(b);
  return pos + hedge.size();
}

void BoxFinder::process(std::string line) {
  // Things to check on a line:
  // 1. find corners
  //    a. if it aligns with a box, do checks then save box
  // 2. check edges for in progress boxes

  std::size_t search = 0;
  while((search = line.find('+', search)) != std::string::npos) {
    // check if it corresponds to an existing box
    // either making an edge or existing within it
    if(onCorner(search) != -1) {
      search += finaliseBox(onCorner(search), line);
    } else if(!contained(search)) {
      search += createBox(search, line);
    }

    // don't find the same corner twice
    search++;
  }

  for(Box b : inProgress) {
    std::string middle = line.substr(b.start(), b.end());
    std::cout << "Adding line to box: " << middle << std::endl;
    b.add(middle);
    b.add(middle);
    std::cout << "Now box contains: " << std::endl;
    b.print();
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
    std::cout << "Usage: ./interpreter input.box" << std::endl;
    return 0;
  }

  std::cout << "testing a string" << std::endl;
  std::vector<std::string> testVec;
  {
    std::string test = "test";
    testVec.push_back(test);
    testVec.push_back(test);
    testVec.push_back(test);
  }
  for(std::string s : testVec) {
    std::cout << s << std::endl;
  }

  std::ifstream input(argv[1]);
  if(input.good()) {
    findFunctions(input);
  }
  input.close();
}
