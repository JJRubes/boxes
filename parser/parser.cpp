#include<iostream>
#include<fstream>
#include<string>
#include<vector>

#include"DefinitionBox.hpp"
#include"UnparsedBox.hpp"

class Program {
  public:
    Program(std::ifstream &program, std::string filename) {
      // placeholder so I don't have to insert later
      contents.push_back("");
      std::size_t longestLine = 0;
      std::string line;
      while(getline(program, line)) {
        // remove carrige returns
        if(line.size() > 0)
          if(line[line.size() - 1] == 0x0D)
            line = line.substr(0, line.size() - 1);

        if(line.size() > longestLine)
          longestLine = line.size();
        contents.push_back(line);
      }

      // "+ file name +" <- the additional spaces are required
      // "|contents[1]|"
      if(filename.size() + 2 > longestLine) {
        longestLine = filename.size() + 2;
      }

      {
        std::string padding(longestLine - filename.size() - 2, '-');
        contents[0] = "+ " + filename + " " + padding + "+";
      }

      // both edges
      for(std::size_t i = 1; i < contents.size(); i++) {
        std::string padding(longestLine - contents[i].size(), ' ');
        contents[i] = "|" + contents[i] + padding + "|";
      }

      {
        std::string padding(longestLine, '-');
        contents.push_back("+" + padding + "+");
      }

      // for(std::string s : contents) {
      //   std::cout << s << std::endl;
      // }

      UnparsedBox ub(1, 1, contents);
      if(!ub.isDef()) {
        std::cout << "Oh no" << std::endl;
      }
      DefinitionBox db = ub.makeDefinition();
      db.print(0, 2);
    }
  private:
    std::vector<std::string> contents;
};

int main(int argc, char* argv[]) {
  std::ifstream input(argv[1]);
  if(input.good()) {
    Program p(input, argv[1]);
  }
  input.close();
}
