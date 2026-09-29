#pragma once

#include "Board.h"

class BoardBuilder {
public:
    virtual ~BoardBuilder() = default;
    virtual void setRenderer(SDL_Renderer *renderer) = 0;
    virtual void setGeometry(int x, int y, int w, int h) = 0;
    virtual Board build() = 0;
};
