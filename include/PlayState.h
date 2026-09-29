#pragma once

#include "GameState.h"

class PlayState : public GameState {
public:
    std::unique_ptr<GameState> onClick(Game &game, int x, int y) override;
};
