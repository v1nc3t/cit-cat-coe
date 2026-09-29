#include "BoardDirector.h"
#include "Game.h"
#include "SdlBoardBuilder.h"

#include <iostream>

namespace
{
bool boardRulesHold()
{
    SdlBoardBuilder builder;
    Board board = BoardDirector(builder).makePlayfield(nullptr, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (board.rect().x != 250 || board.rect().y != 150 || board.rect().w != 300 || board.cellSize() != 100)
        return false;
    if (board.isFull())
        return false;
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            if (board.at(row, col) != MARK_NONE)
                return false;
        }
    }

    Player player;
    if (player.mark != MARK_O || player.winner != MARK_NONE)
        return false;

    const int moves[][2] = {{0, 0}, {1, 0}, {0, 1}, {1, 1}, {0, 2}};
    for (const auto &move : moves)
    {
        if (player.winner != MARK_NONE)
            return false;
        if (!board.fillCell(move[0], move[1], player.mark))
            return false;
        if (board.checkWin(player))
            player.setWinner();
        player.switchPlayer();
    }
    if (player.winner != MARK_O || player.mark != MARK_X)
        return false;
    if (player.winType != 'h' || player.winIndex != 0)
        return false;
    if (board.fillCell(0, 0, MARK_X))
        return false;

    board.reset();
    player.reset();
    player.mark = MARK_X;
    board.fillCell(0, 1, MARK_X);
    board.fillCell(1, 1, MARK_X);
    board.fillCell(2, 1, MARK_X);
    if (!board.checkWin(player) || player.winType != 'v' || player.winIndex != 1)
        return false;

    board.reset();
    player.reset();
    board.fillCell(0, 0, MARK_O);
    board.fillCell(1, 1, MARK_O);
    board.fillCell(2, 2, MARK_O);
    if (!board.checkWin(player) || player.winType != 'm')
        return false;

    board.reset();
    player.reset();
    player.mark = MARK_X;
    board.fillCell(0, 2, MARK_X);
    board.fillCell(1, 1, MARK_X);
    board.fillCell(2, 0, MARK_X);
    if (!board.checkWin(player) || player.winType != 's')
        return false;

    const int draw[3][3] = {
        {MARK_X, MARK_O, MARK_X},
        {MARK_X, MARK_O, MARK_O},
        {MARK_O, MARK_X, MARK_O},
    };
    board.reset();
    player.reset();
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
            board.fillCell(row, col, draw[row][col]);
    }
    if (!board.isFull())
        return false;
    if (board.checkWin(player))
        return false;
    player.mark = MARK_X;
    if (board.checkWin(player))
        return false;
    return true;
}
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    if (!boardRulesHold())
    {
        std::cerr << "board rules check failed" << std::endl;
        return 1;
    }
    Game game;
    if (!game.init())
        return 1;
    game.run();
    return 0;
}
