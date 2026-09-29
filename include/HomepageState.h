#pragma once

#include "GameState.h"

class HomepageState : public GameState {
public:
    std::unique_ptr<GameState> onClick(Game &game, int x, int y) override;
    void render(Game &game) override;
};
