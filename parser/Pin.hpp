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
    const int x();
    const int y();
    DIRECTION getNormal();
    void print(std::size_t indentation, std::size_t indentSize);

  private:
    char identifier;
    int posX, posY;
    DIRECTION normal;
};
