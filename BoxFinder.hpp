#pragma once

#include<iostream>
#include<string>
#include<vector>

#include"UnparsedBox.hpp"

class BoxFinder {
  public:
    BoxFinder();

    // line is the next line from the box
    // that you are parsing.
    // this doesn't include the edges.
    void process(std::string line);
    void printBoxes();
    void move(std::vector<UnparsedBox> boxes);

  private:
    class Box {
      public:
        Box(std::size_t start, std::size_t end, std::size_t lineNo);
        std::size_t start();
        std::size_t end();
        void add(std::string row);
        void print();
        UnparsedBox move();

      private:
        // how far the vertical edges of the box are from
        // the left edge of the file
        // and the line at which the box starts
        std::size_t s, e, l;
        std::vector<std::string> contents;
    };

    std::vector<Box> inProgress;
    std::vector<UnparsedBox> finalised;
    std::size_t lineNo;

    // void finaliseBox();
    void finaliseBox(std::size_t boxIndex);
    void addLine(std::size_t boxIndex, std::string line);
};
