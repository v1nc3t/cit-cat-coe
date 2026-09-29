#include "OnePlayerState.h"

#include "Game.h"
#include "HomepageState.h"

OnePlayerState::OnePlayerState()
    : easyButton_(easyRect_, SDL_Color{255, 255, 255, 255}),
      mediumButton_(mediumRect_, SDL_Color{255, 255, 255, 255}),
      hardButton_(hardRect_, SDL_Color{255, 255, 255, 255})
{
}

void OnePlayerState::start(Game &game, Difficulty difficulty)
{
    difficulty_ = difficulty;
    started_ = true;
    game.board.reset();
    game.player.reset();
    game.citScore = 0;
    game.coeScore = 0;
}

void OnePlayerState::botTurn(Game &game)
{
    if (game.player.winner != MARK_NONE || game.board.isFull() || game.player.mark != MARK_X)
        return;
    int cells[3][3];
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
            cells[row][col] = game.board.at(row, col);
    }
    BotMove move = chooseMove(cells, MARK_X, difficulty_);
    game.placeMark(move.row, move.col);
}

std::unique_ptr<GameState> OnePlayerState::onClick(Game &game, int x, int y)
{
    if (!started_)
    {
        if (game.backButton.isClicked(x, y))
            return std::make_unique<HomepageState>();
        if (easyButton_.isClicked(x, y))
            start(game, Difficulty::Easy);
        else if (mediumButton_.isClicked(x, y))
            start(game, Difficulty::Medium);
        else if (hardButton_.isClicked(x, y))
            start(game, Difficulty::Hard);
        return nullptr;
    }

    if (waitingForBot_)
    {
        if (!game.playAgainButton.isClicked(x, y) && !game.backButton.isClicked(x, y))
            return nullptr;
        waitingForBot_ = false;
    }

    std::unique_ptr<GameState> next = PlayState::onClick(game, x, y);
    if (!next && game.player.winner == MARK_NONE && !game.board.isFull() && game.player.mark == MARK_X)
    {
        waitingForBot_ = true;
        botAt_ = SDL_GetTicks() + 1000;
    }
    return next;
}

void OnePlayerState::render(Game &game)
{
    if (!started_)
    {
        SDL_RenderCopy(game.renderer, game.textures["title"], nullptr, &game.titleRect);
        SDL_RenderCopy(game.renderer, game.textures["cat_stand"], nullptr, &game.catStandRect);
        SDL_RenderCopy(game.renderer, game.textures["backButton_BG"], nullptr, &game.backRect);
        game.backButton.renderButton(game.renderer);
        game.renderFitted(game.easyText, easyRect_);
        game.renderFitted(game.mediumText, mediumRect_);
        game.renderFitted(game.hardText, hardRect_);
        easyButton_.renderButton(game.renderer);
        mediumButton_.renderButton(game.renderer);
        hardButton_.renderButton(game.renderer);
        return;
    }

    if (waitingForBot_ && SDL_TICKS_PASSED(SDL_GetTicks(), botAt_))
    {
        waitingForBot_ = false;
        botTurn(game);
    }

    game.renderMatch();
    SDL_Texture *mode = game.easyText;
    if (difficulty_ == Difficulty::Medium)
        mode = game.mediumText;
    else if (difficulty_ == Difficulty::Hard)
        mode = game.hardText;
    const SDL_Rect modeRect = {280, 12, 240, 40};
    game.renderFitted(mode, modeRect);
}
