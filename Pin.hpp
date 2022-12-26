#pragma once

class Pin {
  public:
    enum class DIRECTION {
      NORTH,
      SOUTH,
      EAST,
      WEST
    };

    Pin(char id, int x, int y, DIRECTION n);
    char getId();
    int x();
    int y();
    DIRECTION getNormal();
    void print();

  private:
    char identifier;
    int posX, posY;
    DIRECTION normal;
};
