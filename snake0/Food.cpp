#include "Food.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

Food::Food() : active(false) {}

Point Food::getPosition() const { return position; }
bool Food::isActive() const { return active; }

void Food::spawn(const vector<vector<Point>>& allSnakeBodies, int maxX, int maxY) {
    vector<Point> validPositions;
    
    for (int x = 2; x < maxX - 2; x++) {
        for (int y = 2; y < maxY - 2; y++) {
            Point candidate(x, y);
            bool occupied = false;
            
            for (const auto& body : allSnakeBodies) {
                for (const Point& segment : body) {
                    if (candidate == segment) {
                        occupied = true;
                        break;
                    }
                }
                if (occupied) break;
            }
            
            if (!occupied) {
                validPositions.push_back(candidate);
            }
        }
    }
    
    if (!validPositions.empty()) {
        position = validPositions[rand() % validPositions.size()];
        active = true;
    }
}

void Food::spawn(const vector<Point>& singleSnakeBody, int maxX, int maxY) {
    vector<vector<Point>> wrapper = { singleSnakeBody };
    spawn(wrapper, maxX, maxY);
}

void Food::deactivate() {
    active = false;
}

void Food::draw() const {
    if (active) {
        gotoxy(position.xCoord, position.yCoord);
        setColor(12);
        cout << "●";
    }
}