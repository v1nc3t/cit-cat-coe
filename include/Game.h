#pragma once

#include "Board.h"
#include "Button.h"
#include "GameState.h"
#include "Player.h"

#include "Sdl.h"

#include <map>
#include <memory>
#include <string>

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

class Game {
    SDL_Window *window_ = nullptr;
    bool sdlReady_ = false;
    bool vsync_ = false;

public:
    SDL_Renderer *renderer = nullptr;
    std::map<std::string, SDL_Texture *> textures;

    SDL_Rect titleRect{};
    SDL_Rect catStandRect{};
    SDL_Rect catSitRect{};
    SDL_Rect twoPlayerRect{};
    SDL_Rect onePlayerRect{};
    SDL_Rect backRect{};
    SDL_Rect playAgainRect{};
    SDL_Rect citRect{};
    SDL_Rect coeRect{};

    Button onePlayerButton;
    Button twoPlayerButton;
    Button backButton;
    Button playAgainButton;

    Board board;
    Player player;
    std::unique_ptr<GameState> state;

    ~Game();
    bool init();
    void run();
};
