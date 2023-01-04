#include<string>
#include<vector>

#include"DefinitionBox.hpp"
#include"CallBox.hpp"
#include"Connection.hpp"
#include"Pin.hpp"

DefinitionBox::DefinitionBox(std::string n,
                             std::vector<Pin> p,
                             std::vector<DefinitionBox> defs,
                             std::vector<CallBox> cls,
                             std::vector<Connection> cns) {
  name = n;
  pins.swap(p);
  definitions.swap(defs);
  calls.swap(cls);
  connections.swap(cns);
}
