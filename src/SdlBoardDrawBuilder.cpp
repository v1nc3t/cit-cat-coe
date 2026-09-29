#include "SdlBoardDrawBuilder.h"

#include <cmath>

SdlBoardDrawBuilder::SdlBoardDrawBuilder(const Board &board, const Player &player)
    : renderer_(board.renderer()), board_(board), player_(player) {}

void SdlBoardDrawBuilder::drawX(int x1, int y1, int x2, int y2)
{
    SDL_RenderDrawLine(renderer_, x1, y1, x2, y2);
    SDL_RenderDrawLine(renderer_, x2, y1, x1, y2);
}

void SdlBoardDrawBuilder::drawO(int x, int y, int radius)
{
    const double pi = std::acos(-1.0);
    for (int angle = 0; angle < 360; angle++)
    {
        int dx = static_cast<int>(radius * std::cos(angle * pi / 180.0));
        int dy = static_cast<int>(radius * std::sin(angle * pi / 180.0));
        SDL_RenderDrawPoint(renderer_, x + dx, y + dy);
    }
}

void SdlBoardDrawBuilder::background()
{
    SDL_SetRenderDrawColor(renderer_, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer_);
}

void SdlBoardDrawBuilder::grid()
{
    SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
    const SDL_Rect &rect = board_.rect();
    const int cell = board_.cellSize();
    for (int i = 1; i < 3; i++)
        SDL_RenderDrawLine(renderer_, rect.x, rect.y + i * cell, rect.x + rect.w, rect.y + i * cell);
    for (int i = 1; i < 3; i++)
        SDL_RenderDrawLine(renderer_, rect.x + i * cell, rect.y, rect.x + i * cell, rect.y + rect.w);
}

void SdlBoardDrawBuilder::marks()
{
    SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
    const SDL_Rect &rect = board_.rect();
    const int cell = board_.cellSize();
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            const int x = rect.x + j * cell + cell / 2;
            const int y = rect.y + i * cell + cell / 2;
            const int mark = board_.at(i, j);
            if (mark == MARK_X)
                drawX(x - cell / 4, y - cell / 4, x + cell / 4, y + cell / 4);
            else if (mark == MARK_O)
                drawO(x, y, cell / 4);
        }
    }
}

void SdlBoardDrawBuilder::winLine()
{
    if (player_.winner == MARK_NONE)
        return;

    SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
    const SDL_Rect &rect = board_.rect();
    const int cell = board_.cellSize();
    if (player_.winType == 'm')
    {
        SDL_RenderDrawLine(renderer_, rect.x, rect.y, rect.x + rect.w, rect.y + rect.w);
    }
    else if (player_.winType == 's')
    {
        SDL_RenderDrawLine(renderer_, rect.x + rect.w, rect.y, rect.x, rect.y + rect.w);
    }
    else if (player_.winType == 'h')
    {
        const int y = rect.y + static_cast<int>((player_.winIndex + 0.5) * cell);
        SDL_RenderDrawLine(renderer_, rect.x, y, rect.x + rect.w, y);
    }
    else if (player_.winType == 'v')
    {
        const int x = rect.x + static_cast<int>((player_.winIndex + 0.5) * cell);
        SDL_RenderDrawLine(renderer_, x, rect.y, x, rect.y + rect.w);
    }
}
