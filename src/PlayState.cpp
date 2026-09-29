#include "PlayState.h"

#include "Game.h"
#include "HomepageState.h"

std::unique_ptr<GameState> PlayState::onClick(Game &game, int x, int y)
{
    std::unique_ptr<GameState> next;
    if (game.playAgainButton.isClicked(x, y) || game.backButton.isClicked(x, y))
    {
        game.board.reset();
        game.player.reset();
        if (game.backButton.isClicked(x, y))
            next = std::make_unique<HomepageState>();
    }
    if (game.board.isClicked(x, y) && game.player.winner == MARK_NONE)
    {
        int row, col;
        game.board.findCell(x, y, row, col);
        if (game.board.fillCell(row, col, game.player.mark))
        {
            if (game.board.checkWin(game.player))
            {
                game.player.setWinner();
                if (game.player.winner == MARK_O)
                    game.citScore++;
                else
                    game.coeScore++;
            }
            game.player.switchPlayer();
        }
    }
    return next;
}
