#pragma once

#include "Sdl.h"

class Button {
    SDL_Rect rect_{};
    SDL_Color outline_{};

public:
    Button() = default;
    Button(SDL_Rect imgRect, SDL_Color borderColor);
    bool isClicked(int mouseX, int mouseY) const;
    void renderButton(SDL_Renderer *renderer) const;
};
