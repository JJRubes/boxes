#pragma once

#include<string>
#include<vector>

#include"CallBox.hpp"
#include"DefinitionBox.hpp"
#include"Pin.hpp"

class UnparsedBox {
  public:
    UnparsedBox(int x, int y, std::vector<std::string> lines);
    bool isDef();
    DefinitionBox makeDefinition();
    CallBox makeCall();
    void print();

  private:
    void parseEdges();

    const bool isPin(char p);
    char at(int x, int y);
    void parseName();
    int scanHorizontal(int line, bool forwards);
    int scanVertical(int line, bool forwards);
    void getName(int start, int end, int line, bool horizontal);

    bool edgesParsed;
    // makes it easier to handle the
    // def/call distinction
    bool definition;

    std::string name;

    // top left relative to parent box
    int tlx, tly;

    // width/height stored for easy access
    int width, height;

    // the string data of the box
    std::vector<std::string> contents;

    // the box's pins
    std::vector<Pin> pins;
};
