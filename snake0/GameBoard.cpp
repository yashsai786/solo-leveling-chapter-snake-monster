#include "GameBoard.h"
#include <iostream>
using namespace std;

static const PlayerConfig PLAYER_CONFIGS[2] = {
    { consoleWidth / 4, consoleHeight / 2, DIR_RIGHT, 10, 2 },       // Player 1: Green, Arrow keys
    { (3 * consoleWidth) / 4, consoleHeight / 2, DIR_LEFT, 11, 3 }   // Player 2: Cyan, WASD keys
};

GameBoard::GameBoard(HighScoreManager* hsm, int difficulty, const string& name) : 
    level(1), gameOver(false), loserPlayer(-1), highScoreManager(hsm), playerName(name) {
    srand(static_cast<unsigned int>(time(0)));
    initScreen();
    
    switch(difficulty) {
        case 1: baseSpeed = 180; break;
        case 2: baseSpeed = 120; break;
        case 3: baseSpeed = 70;  break;
        default: baseSpeed = 120; break;
    }
    currentSpeed = baseSpeed;
    
    for (int i = 0; i < NUM_PLAYERS; i++) {
        scores[i] = 0;
        snakes[i] = new Snake(PLAYER_CONFIGS[i].startX, PLAYER_CONFIGS[i].startY, 3,
                              PLAYER_CONFIGS[i].startDir, PLAYER_CONFIGS[i].headColor, PLAYER_CONFIGS[i].bodyColor);
    }

    system("cls");
    drawBorder();
    displayInstructions();
}

GameBoard::~GameBoard() { 
    for (int i = 0; i < NUM_PLAYERS; i++) {
        delete snakes[i];
    }
}

int GameBoard::getScore(int playerIdx) const { 
    if (playerIdx >= 0 && playerIdx < NUM_PLAYERS) return scores[playerIdx];
    return 0;
}
int GameBoard::getLevel() const { return level; }
int GameBoard::getSpeed() const { return currentSpeed; }
bool GameBoard::isGameOver() const { return gameOver; }

void GameBoard::updateLevel() {
    int maxScore = max(scores[0], scores[1]);
    int newLevel = (maxScore / 10) + 1;
    if (newLevel > level) {
        level = newLevel;
        currentSpeed = max(30, baseSpeed - (level - 1) * (baseSpeed / 10));
    }
}

void GameBoard::spawnFood() {
    vector<vector<Point>> allBodies;
    for (int i = 0; i < NUM_PLAYERS; i++) {
        allBodies.push_back(snakes[i]->getBody());
    }
    food.spawn(allBodies, consoleWidth, consoleHeight);
}

void GameBoard::drawBorder() {
    setColor(14);
    for (int x = 0; x < consoleWidth; x++) {
        gotoxy(x, 0); cout << "═";
        gotoxy(x, consoleHeight - 1); cout << "═";
    }
    for (int y = 0; y < consoleHeight; y++) {
        gotoxy(0, y); cout << "║";
        gotoxy(consoleWidth - 1, y); cout << "║";
    }
    
    gotoxy(0, 0); cout << "╔";
    gotoxy(consoleWidth - 1, 0); cout << "╗";
    gotoxy(0, consoleHeight - 1); cout << "╚";
    gotoxy(consoleWidth - 1, consoleHeight - 1); cout << "╝";
    
    setColor(7);
}

void GameBoard::displayGameInfo() {
    gotoxy(2, 0);
    setColor(10);
    cout << "P1 (Arrows): " << scores[0];
    
    gotoxy(consoleWidth / 2 - 8, 0);
    setColor(11);
    cout << "P2 (WASD): " << scores[1];
    
    gotoxy(consoleWidth / 2 + 10, 0);
    setColor(13);
    cout << "Level: " << level;
    
    gotoxy(consoleWidth - 18, 0);
    setColor(14);
    cout << "Speed: " << currentSpeed << "ms";
}

void GameBoard::displayInstructions() {
    gotoxy(2, consoleHeight - 1);
    setColor(8);
    cout << "P1: Arrow Keys | P2: W/A/S/D | P: Pause";
}

void GameBoard::showLevelUpMessage() {
    int centerX = consoleWidth / 2 - 10;
    int centerY = consoleHeight / 2;
    MessageBeep(MB_OK);
    gotoxy(centerX, centerY - 1);
    setColor(14);
    cout << "╔══════════════════════╗";
    
    gotoxy(centerX, centerY);
    setColor(14);
    cout << "║      LEVEL UP!       ║";
    
    gotoxy(centerX, centerY + 1);
    setColor(14);
    cout << "║   Now at Level " << setw(2) << level << "   ║";
    
    gotoxy(centerX, centerY + 2);
    setColor(14);
    cout << "╚══════════════════════╝";
    
    for (int i = 0; i < 3; i++) {
        Sleep(300);
        gotoxy(centerX, centerY);
        setColor(12);
        cout << "║      LEVEL UP!       ║";
        Sleep(300);
        gotoxy(centerX, centerY);
        setColor(14);
        cout << "║      LEVEL UP!       ║";
    }
    
    for (int y = centerY - 1; y <= centerY + 2; y++) {
        gotoxy(centerX, y);
        cout << "                      ";
    }
}

void GameBoard::draw() {
    displayGameInfo();
    for (int i = 0; i < NUM_PLAYERS; i++) {
        snakes[i]->draw();
    }
    food.draw();
}

bool GameBoard::update() {
    if (gameOver) return false;
    
    bool foodEatenAny = false;
    for (int i = 0; i < NUM_PLAYERS; i++) {
        snakes[i]->clearTail();
        if (snakes[i]->move(food)) {
            scores[i]++;
            foodEatenAny = true;
        }
    }
    
    if (foodEatenAny) {
        MessageBeep(MB_ICONASTERISK);
        int oldLevel = level;
        updateLevel();
        
        if (level > oldLevel) {
            draw();
            showLevelUpMessage();
            draw();
        }
        
        spawnFood();
    }
    
    bool p1Died = snakes[0]->checkSelfCollision() || 
                  snakes[0]->checkBoundaryCollision(consoleWidth, consoleHeight) ||
                  snakes[0]->checkOtherCollision(*snakes[1]);
                  
    bool p2Died = snakes[1]->checkSelfCollision() || 
                  snakes[1]->checkBoundaryCollision(consoleWidth, consoleHeight) ||
                  snakes[1]->checkOtherCollision(*snakes[0]);
                  
    if (p1Died || p2Died) {
        gameOver = true;
        if (p1Died && p2Died) loserPlayer = 2; // Both died / Draw
        else if (p1Died) loserPlayer = 0;      // Player 1 died
        else loserPlayer = 1;                  // Player 2 died
        return false;
    }
    
    return true;
}

void GameBoard::getInput() {
    while (_kbhit()) {
        int key = _getch();

        if (key == 0 || key == 224) {
            key = _getch();
            switch (key) {
                case 72: snakes[0]->changeDirection(DIR_UP); break;
                case 80: snakes[0]->changeDirection(DIR_DOWN); break;
                case 75: snakes[0]->changeDirection(DIR_LEFT); break;
                case 77: snakes[0]->changeDirection(DIR_RIGHT); break;
            }
        } else {
            switch (tolower(key)) {
                case 'w': snakes[1]->changeDirection(DIR_UP); break;
                case 'a': snakes[1]->changeDirection(DIR_LEFT); break;
                case 's': snakes[1]->changeDirection(DIR_DOWN); break;
                case 'd': snakes[1]->changeDirection(DIR_RIGHT); break;
                case 'p':
                    gotoxy(consoleWidth / 2 - 5, consoleHeight / 2);
                    setColor(14);
                    cout << "PAUSED";
                    while (_getch() != 'p') {}
                    break;
            }
        }
    }
}

void GameBoard::displayGameOver() {
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    MessageBeep(MB_ICONHAND);
    setColor(12);
    cout << "\n\n";
    cout << "   ╔══════════════════════════════╗\n";
    cout << "   ║         GAME OVER!           ║\n";
    cout << "   ╚══════════════════════════════╝\n\n";
    
    setColor(7);
    cout << "   Player 1 (Green) Score: " << scores[0] << endl;
    cout << "   Player 2 (Cyan)  Score: " << scores[1] << endl;
    cout << "   Level Reached: " << level << endl << endl;

    if (loserPlayer == 0) {
        setColor(11);
        cout << "   🏆 PLAYER 2 WINS! (Player 1 crashed)\n";
    } else if (loserPlayer == 1) {
        setColor(10);
        cout << "   🏆 PLAYER 1 WINS! (Player 2 crashed)\n";
    } else {
        setColor(14);
        cout << "   ⚔️ IT'S A DRAW! (Both players collided)\n";
    }
    
    int topMatchScore = max(scores[0], scores[1]);
    int highest = highScoreManager->getHighestScore();
    if (topMatchScore > highest && topMatchScore > 0) {
        string winnerName = (scores[0] >= scores[1]) ? (playerName + "_P1") : (playerName + "_P2");
        highScoreManager->addHighScore(winnerName, topMatchScore);
        setColor(14);
        cout << "\n   🎉 NEW HIGH SCORE: " << topMatchScore << "! 🎉\n";
    }
    
    setColor(10);
    cout << "\n   Press any key to continue...";
    _getch();
}