#include "TwoPlayerState.h"

#include "BoardDrawDirector.h"
#include "Game.h"
#include "SdlBoardDrawBuilder.h"

void TwoPlayerState::render(Game &game)
{
    SdlBoardDrawBuilder drawBuilder(game.board, game.player);
    BoardDrawDirector().draw(drawBuilder);

    SDL_RenderCopy(game.renderer, game.textures["backButton_BG"], nullptr, &game.backRect);
    game.backButton.renderButton(game.renderer);
    SDL_RenderCopy(game.renderer, game.textures["cat_sit"], nullptr, &game.catSitRect);

    if (game.board.isFull())
    {
        SDL_RenderCopy(game.renderer, game.textures["playAgain"], nullptr, &game.playAgainRect);
        game.playAgainButton.renderButton(game.renderer);
        SDL_RenderCopy(game.renderer, game.textures["cit"], nullptr, &game.citRect);
        SDL_RenderCopy(game.renderer, game.textures["coe"], nullptr, &game.coeRect);
    }
    else if (game.player.winner == MARK_NONE)
    {
        SDL_RenderCopy(game.renderer, game.textures[game.player.mark == MARK_O ? "cit_turn" : "cit"], nullptr, &game.citRect);
        SDL_RenderCopy(game.renderer, game.textures[game.player.mark == MARK_O ? "coe" : "coe_turn"], nullptr, &game.coeRect);
    }
    else
    {
        SDL_RenderCopy(game.renderer, game.textures["playAgain"], nullptr, &game.playAgainRect);
        game.playAgainButton.renderButton(game.renderer);
        if (game.player.winner == MARK_O)
            game.renderFitted(game.citWinText, game.citRect);
        else
            SDL_RenderCopy(game.renderer, game.textures["cit"], nullptr, &game.citRect);
        if (game.player.winner == MARK_X)
            game.renderFitted(game.coeWinText, game.coeRect);
        else
            SDL_RenderCopy(game.renderer, game.textures["coe"], nullptr, &game.coeRect);
    }
    game.renderScore(game.citScoreLabel, game.citScore, game.citRect);
    game.renderScore(game.coeScoreLabel, game.coeScore, game.coeRect);
}
