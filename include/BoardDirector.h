#pragma once

#include "BoardBuilder.h"

class BoardDirector {
    BoardBuilder &builder_;

public:
    explicit BoardDirector(BoardBuilder &builder);
    Board makePlayfield(SDL_Renderer *renderer, int screenW, int screenH);
};
