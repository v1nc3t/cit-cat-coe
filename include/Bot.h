#pragma once

#include "Mark.h"

enum class Difficulty { Easy, Medium, Hard };

struct BotMove {
    int row;
    int col;
};

BotMove chooseMove(const int cells[3][3], int mark, Difficulty difficulty);
bool botRulesHold();
