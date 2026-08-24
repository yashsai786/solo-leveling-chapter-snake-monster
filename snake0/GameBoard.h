#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "Common.h"
#include "Snake.h"
#include "Food.h"
#include "HighScoreManager.h"
#include <conio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>

struct PlayerConfig {
    int startX;
    int startY;
    char startDir;
    int headColor;
    int bodyColor;
};

class GameBoard {
private:
    static constexpr int NUM_PLAYERS = 1; // Single source of truth for snake count
    Snake* snakes[NUM_PLAYERS];
    int scores[NUM_PLAYERS];
    Food food;
    int level;
    int baseSpeed;
    int currentSpeed;
    bool gameOver;
    HighScoreManager* highScoreManager;
    string playerName;

public:
    GameBoard(HighScoreManager* hsm, int difficulty, const string& name);
    ~GameBoard();
    int getScore(int playerIdx = 0) const;
    int getLevel() const;
    int getSpeed() const;
    bool isGameOver() const;
    void updateLevel();
    void spawnFood();
    void drawBorder();
    void displayGameInfo();
    void displayInstructions();
    void showLevelUpMessage();
    void draw();
    bool update();
    void getInput();
    void displayGameOver();
};

#endif