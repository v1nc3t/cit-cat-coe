#include "Button.h"

Button::Button(SDL_Rect imgRect, SDL_Color borderColor) : rect_(imgRect), outline_(borderColor) {}

bool Button::isClicked(int mouseX, int mouseY) const
{
    SDL_Point p = {mouseX, mouseY};
    return SDL_PointInRect(&p, &rect_);
}

void Button::renderButton(SDL_Renderer *renderer) const
{
    SDL_SetRenderDrawColor(renderer, outline_.r, outline_.g, outline_.b, outline_.a);
    SDL_RenderDrawRect(renderer, &rect_);
}
