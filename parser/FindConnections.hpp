#pragma once

#include<string>
#include<vector>

#include"Connection.hpp"
#include"CallBox.hpp"
#include"DefinitionBox.hpp"
#include"Pin.hpp"

class FindConnections {
  public:
    FindConnections(const std::vector<std::string> &c,
                    const std::vector<Pin> &p,
                    const std::vector<CallBox> &cb,
                    const std::vector<DefinitionBox> &db,
                    int x, int y);
    void process();
    void print();

  private:
    std::vector<std::string> contents;
    std::vector<Pin> pins;
    std::vector<DefinitionBox> definitions;
    std::vector<CallBox> calls;
    std::vector<Connection> connections;
    int selfOffsetX, selfOffsetY;

    bool connected(Connection::BoxType t, size_t b, size_t p);
    void findPath(Connection::BoxType t, size_t b, size_t p);
    void printMap(int x, int y);
    Pin getPin(Connection::BoxType t, size_t b, size_t p);
    int getPinX(Connection::BoxType t, size_t b, size_t p);
    int getPinY(Connection::BoxType t, size_t b, size_t p);
    bool makeConnection(Connection::BoxType t, size_t b, size_t p, std::string typeInfo, bool ordered, int x, int y);
};
