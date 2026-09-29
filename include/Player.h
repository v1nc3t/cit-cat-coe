#pragma once

#include "Mark.h"

class Player {
public:
    int mark;
    int winner;
    char winType;
    int winIndex;

    Player();
    void reset();
    void switchPlayer();
    void setWinner();
};
