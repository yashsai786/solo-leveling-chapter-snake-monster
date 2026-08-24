#ifndef SNAKE_H
#define SNAKE_H

#include "Common.h"
#include "Food.h"
#include <vector>

class Snake {
private:
    vector<Point> body;
    char direction;
    int length;
    int headColor;
    int bodyColor;

public:
    Snake(int x, int y, int initialLength = 3, char initialDirection = DIR_RIGHT, int hColor = 10, int bColor = 2);
    const vector<Point>& getBody() const;
    int getLength() const;
    char getDirection() const;
    Point getHead() const;
    void changeDirection(char newDirection);
    bool move(Food& food);
    bool checkSelfCollision() const;
    bool checkOtherCollision(const Snake& other) const;
    bool checkBoundaryCollision(int maxX, int maxY) const;
    void draw() const;
    void clearTail() const;
};

#endif