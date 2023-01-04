#pragma once

#include<string>
#include<vector>

#include"Connection.hpp"
#include"Pin.hpp"

class FindConnections {
  public:
    FindConnections(std::vector<std::string> c, std::vector<Pin> p);
    void process();
    void print();
    void move(std::vector<std::string> lines, std::vector<Pin> p, std::vector<Connection> cns);

  private:
    std::vector<std::string> contents;
    std::vector<Pin> pins;
    std::vector<Connection> connections;

    bool connected(std::size_t index);
    void findPath(std::size_t index);
    void printMap(int x, int y);
};
