#include<string>
#include<vector>

#include"CallBox.hpp"
#include"UnparsedBox.hpp"

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
//  if(!b.isDef()) {
//    CallBox cb = b.makeCall();
//    cb.print();
//  }
}
