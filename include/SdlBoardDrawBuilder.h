#pragma once

#include "Board.h"
#include "BoardDrawBuilder.h"
#include "Player.h"

class SdlBoardDrawBuilder : public BoardDrawBuilder {
    SDL_Renderer *renderer_;
    const Board &board_;
    const Player &player_;

    void drawX(int x1, int y1, int x2, int y2);
    void drawO(int x, int y, int radius);

public:
    SdlBoardDrawBuilder(const Board &board, const Player &player);
    void background() override;
    void grid() override;
    void marks() override;
    void winLine() override;
};
