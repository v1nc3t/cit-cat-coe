#include "BoardDirector.h"

BoardDirector::BoardDirector(BoardBuilder &builder) : builder_(builder) {}

Board BoardDirector::makePlayfield(SDL_Renderer *renderer, int screenW, int screenH)
{
    const int side = 300;
    builder_.setRenderer(renderer);
    builder_.setGeometry((screenW - side) / 2, (screenH - side) / 2, side, side);
    return builder_.build();
}
