#include "SdlBoardBuilder.h"

void SdlBoardBuilder::setRenderer(SDL_Renderer *renderer)
{
    renderer_ = renderer;
}

void SdlBoardBuilder::setGeometry(int x, int y, int w, int h)
{
    x_ = x;
    y_ = y;
    w_ = w;
    h_ = h;
}

Board SdlBoardBuilder::build()
{
    Board board;
    board.renderer_ = renderer_;
    board.rect_ = {x_, y_, w_, h_};
    board.cellSize_ = w_ / 3;
    board.reset();
    return board;
}
