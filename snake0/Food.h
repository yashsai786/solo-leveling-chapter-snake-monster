#ifndef FOOD_H
#define FOOD_H

#include "Common.h"
#include <vector>

class Food {
private:
    Point position;
    bool active;

public:
    Food();
    Point getPosition() const;
    bool isActive() const;
    void spawn(const vector<vector<Point>>& allSnakeBodies, int maxX, int maxY);
    void spawn(const vector<Point>& singleSnakeBody, int maxX, int maxY);
    void deactivate();
    void draw() const;
};

#endif