#include "Player.h"

Player::Player()
{
    reset();
}

void Player::reset()
{
    mark = MARK_O;
    winner = MARK_NONE;
    winType = '-';
    winIndex = 0;
}

void Player::switchPlayer()
{
    mark = (mark == MARK_O) ? MARK_X : MARK_O;
}

void Player::setWinner()
{
    winner = mark;
}
