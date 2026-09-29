#pragma once

#include "Mark.h"

#include "Sdl.h"

class Player;
class SdlBoardBuilder;

class Board {
    friend class SdlBoardBuilder;

    SDL_Renderer *renderer_ = nullptr;
    SDL_Rect rect_{};
    int cellSize_ = 0;
    int cells_[3][3]{};

public:
    Board();

    SDL_Renderer *renderer() const { return renderer_; }
    const SDL_Rect &rect() const { return rect_; }
    int cellSize() const { return cellSize_; }
    int at(int row, int col) const { return cells_[row][col]; }

    void reset();
    bool isFull() const;
    bool isClicked(int mouseX, int mouseY) const;
    void findCell(int mouseX, int mouseY, int &row, int &col) const;
    bool fillCell(int row, int col, int mark);
    bool checkWin(Player &curr) const;
};
