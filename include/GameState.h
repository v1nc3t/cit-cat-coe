#pragma once

#include <memory>

class Game;

class GameState {
public:
    virtual ~GameState() = default;
    virtual std::unique_ptr<GameState> onClick(Game &game, int x, int y) = 0;
    virtual void render(Game &game) = 0;
};
