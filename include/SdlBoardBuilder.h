#pragma once

#include "BoardBuilder.h"

class SdlBoardBuilder : public BoardBuilder {
    SDL_Renderer *renderer_ = nullptr;
    int x_ = 0;
    int y_ = 0;
    int w_ = 0;
    int h_ = 0;

public:
    void setRenderer(SDL_Renderer *renderer) override;
    void setGeometry(int x, int y, int w, int h) override;
    Board build() override;
};
