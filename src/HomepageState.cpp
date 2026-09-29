#include "HomepageState.h"

#include "Game.h"
#include "OnePlayerState.h"
#include "TwoPlayerState.h"

std::unique_ptr<GameState> HomepageState::onClick(Game &game, int x, int y)
{
    std::unique_ptr<GameState> next;
    if (game.onePlayerButton.isClicked(x, y))
        next = std::make_unique<OnePlayerState>();
    if (game.twoPlayerButton.isClicked(x, y))
        next = std::make_unique<TwoPlayerState>();
    return next;
}

void HomepageState::render(Game &game)
{
    SDL_RenderCopy(game.renderer, game.textures["title"], nullptr, &game.titleRect);
    SDL_RenderCopy(game.renderer, game.textures["cat_stand"], nullptr, &game.catStandRect);
    SDL_RenderCopy(game.renderer, game.textures["twoPlayer_BG"], nullptr, &game.twoPlayerRect);
    SDL_RenderCopy(game.renderer, game.textures["onePlayer_BG"], nullptr, &game.onePlayerRect);
    game.onePlayerButton.renderButton(game.renderer);
    game.twoPlayerButton.renderButton(game.renderer);
}
