#pragma once

#include "Bot.h"
#include "Button.h"
#include "PlayState.h"

class OnePlayerState : public PlayState {
    bool started_ = false;
    bool waitingForBot_ = false;
    Uint32 botAt_ = 0;
    Difficulty difficulty_ = Difficulty::Easy;
    SDL_Rect easyRect_{300, 210, 200, 64};
    SDL_Rect mediumRect_{300, 290, 200, 64};
    SDL_Rect hardRect_{300, 370, 200, 64};
    Button easyButton_;
    Button mediumButton_;
    Button hardButton_;

    void start(Game &game, Difficulty difficulty);
    void botTurn(Game &game);

public:
    OnePlayerState();
    std::unique_ptr<GameState> onClick(Game &game, int x, int y) override;
    void render(Game &game) override;
};
