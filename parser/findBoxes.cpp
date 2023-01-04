#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include"BoxFinder.hpp"

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
