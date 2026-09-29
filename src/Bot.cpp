#include "Bot.h"

#include <random>

namespace
{
std::mt19937 &rng()
{
    static std::mt19937 gen{std::random_device{}()};
    return gen;
}

int roll(int count)
{
    std::uniform_int_distribution<int> dist(0, count - 1);
    return dist(rng());
}

int lineWinner(int a, int b, int c)
{
    if (a != MARK_NONE && a == b && b == c)
        return a;
    return MARK_NONE;
}

int gridWinner(const int cells[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        int row = lineWinner(cells[i][0], cells[i][1], cells[i][2]);
        if (row != MARK_NONE)
            return row;
        int col = lineWinner(cells[0][i], cells[1][i], cells[2][i]);
        if (col != MARK_NONE)
            return col;
    }
    int main = lineWinner(cells[0][0], cells[1][1], cells[2][2]);
    if (main != MARK_NONE)
        return main;
    return lineWinner(cells[0][2], cells[1][1], cells[2][0]);
}

struct Cell {
    int row;
    int col;
};

int emptyCells(const int cells[3][3], Cell *out)
{
    int count = 0;
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            if (cells[row][col] == MARK_NONE)
                out[count++] = {row, col};
        }
    }
    return count;
}

int other(int mark)
{
    return mark == MARK_O ? MARK_X : MARK_O;
}

int minimax(int cells[3][3], int mark, int bot)
{
    int winner = gridWinner(cells);
    if (winner == bot)
        return 1;
    if (winner != MARK_NONE)
        return -1;

    Cell open[9];
    int count = emptyCells(cells, open);
    if (count == 0)
        return 0;

    int best = mark == bot ? -2 : 2;
    for (int i = 0; i < count; i++)
    {
        cells[open[i].row][open[i].col] = mark;
        int score = minimax(cells, other(mark), bot);
        cells[open[i].row][open[i].col] = MARK_NONE;
        if (mark == bot)
            best = score > best ? score : best;
        else
            best = score < best ? score : best;
    }
    return best;
}

BotMove perfectMove(int cells[3][3], int mark)
{
    Cell open[9];
    int count = emptyCells(cells, open);
    Cell best[9];
    int bestCount = 0;
    int bestScore = -2;
    for (int i = 0; i < count; i++)
    {
        cells[open[i].row][open[i].col] = mark;
        int score = minimax(cells, other(mark), mark);
        cells[open[i].row][open[i].col] = MARK_NONE;
        if (score > bestScore)
        {
            bestScore = score;
            bestCount = 0;
        }
        if (score == bestScore)
            best[bestCount++] = open[i];
    }
    Cell pick = best[roll(bestCount)];
    return {pick.row, pick.col};
}
}

BotMove chooseMove(const int cells[3][3], int mark, Difficulty difficulty)
{
    int scratch[3][3];
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
            scratch[row][col] = cells[row][col];
    }

    Cell open[9];
    int count = emptyCells(scratch, open);
    if (count == 0)
        return {-1, -1};

    // ponytail: medium blunders 1 move in 3; lower the 3 if it should miss less often
    if (difficulty == Difficulty::Easy || (difficulty == Difficulty::Medium && roll(3) == 0))
    {
        Cell pick = open[roll(count)];
        return {pick.row, pick.col};
    }
    return perfectMove(scratch, mark);
}

bool botRulesHold()
{
    int winNow[3][3] = {
        {MARK_X, MARK_X, MARK_NONE},
        {MARK_O, MARK_O, MARK_NONE},
        {MARK_NONE, MARK_NONE, MARK_NONE},
    };
    BotMove taken = chooseMove(winNow, MARK_X, Difficulty::Hard);
    if (taken.row != 0 || taken.col != 2)
        return false;

    int mustBlock[3][3] = {
        {MARK_O, MARK_O, MARK_NONE},
        {MARK_NONE, MARK_X, MARK_NONE},
        {MARK_NONE, MARK_NONE, MARK_NONE},
    };
    BotMove block = chooseMove(mustBlock, MARK_X, Difficulty::Hard);
    if (block.row != 0 || block.col != 2)
        return false;

    int only[3][3] = {
        {MARK_O, MARK_X, MARK_O},
        {MARK_X, MARK_O, MARK_O},
        {MARK_X, MARK_O, MARK_NONE},
    };
    BotMove last = chooseMove(only, MARK_X, Difficulty::Easy);
    if (last.row != 2 || last.col != 2)
        return false;

    int board[3][3] = {
        {MARK_NONE, MARK_NONE, MARK_NONE},
        {MARK_NONE, MARK_NONE, MARK_NONE},
        {MARK_NONE, MARK_NONE, MARK_NONE},
    };
    int mark = MARK_O;
    for (int turn = 0; turn < 9; turn++)
    {
        if (gridWinner(board) != MARK_NONE)
            return false;
        BotMove move = chooseMove(board, mark, Difficulty::Hard);
        if (move.row < 0 || move.col < 0 || board[move.row][move.col] != MARK_NONE)
            return false;
        board[move.row][move.col] = mark;
        mark = other(mark);
    }
    return gridWinner(board) == MARK_NONE;
}
