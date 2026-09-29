#include "Board.h"

#include "Player.h"

Board::Board()
{
    reset();
}

void Board::reset()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
            cells_[i][j] = MARK_NONE;
    }
}

bool Board::isFull() const
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (cells_[i][j] == MARK_NONE)
                return false;
        }
    }
    return true;
}

bool Board::isClicked(int mouseX, int mouseY) const
{
    SDL_Point p = {mouseX, mouseY};
    return SDL_PointInRect(&p, &rect_);
}

void Board::findCell(int mouseX, int mouseY, int &row, int &col) const
{
    row = (mouseY - rect_.y) / cellSize_;
    col = (mouseX - rect_.x) / cellSize_;
}

bool Board::fillCell(int row, int col, int mark)
{
    if (cells_[row][col] == MARK_NONE)
    {
        cells_[row][col] = mark;
        return true;
    }
    return false;
}

bool Board::checkWin(Player &curr) const
{
    for (int i = 0; i < 3; ++i)
    {
        if (cells_[i][0] == curr.mark && cells_[i][1] == curr.mark && cells_[i][2] == curr.mark)
        {
            curr.winIndex = i;
            curr.winType = 'h';
            return true;
        }
        if (cells_[0][i] == curr.mark && cells_[1][i] == curr.mark && cells_[2][i] == curr.mark)
        {
            curr.winIndex = i;
            curr.winType = 'v';
            return true;
        }
    }
    if (cells_[0][0] == curr.mark && cells_[1][1] == curr.mark && cells_[2][2] == curr.mark)
    {
        curr.winType = 'm';
        return true;
    }
    if (cells_[0][2] == curr.mark && cells_[1][1] == curr.mark && cells_[2][0] == curr.mark)
    {
        curr.winType = 's';
        return true;
    }
    return false;
}
