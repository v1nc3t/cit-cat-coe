#include "Game.h"

#include "BoardDirector.h"
#include "HomepageState.h"
#include "SdlBoardBuilder.h"

#include <iostream>

namespace
{
SDL_Texture *loadImage(SDL_Renderer *renderer, const char *filePath)
{
    SDL_Surface *surface = SDL_LoadBMP(filePath);
    if (!surface)
    {
        std::cerr << "Unable to load image: " << filePath << "! SDL_Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}
}

Game::~Game()
{
    for (auto &pair : textures)
    {
        if (pair.second)
            SDL_DestroyTexture(pair.second);
    }
    textures.clear();
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window_)
        SDL_DestroyWindow(window_);
    renderer = nullptr;
    window_ = nullptr;
    if (sdlReady_)
        SDL_Quit();
}

bool Game::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    sdlReady_ = true;

    window_ = SDL_CreateWindow("Cit Cat Coe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window_)
    {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
        renderer = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    SDL_RendererInfo info;
    if (SDL_GetRendererInfo(renderer, &info) == 0)
        vsync_ = (info.flags & SDL_RENDERER_PRESENTVSYNC) != 0;

    SDL_Surface *icon = SDL_LoadBMP("assets/icon.bmp");
    if (icon)
    {
        SDL_SetWindowIcon(window_, icon);
        SDL_FreeSurface(icon);
    }

    const struct
    {
        const char *key;
        const char *path;
    } assets[] = {
        {"title", "assets/title.bmp"},
        {"cat_stand", "assets/cat_stand.bmp"},
        {"cat_sit", "assets/cat_sit.bmp"},
        {"twoPlayer_BG", "assets/twoplayer.bmp"},
        {"onePlayer_BG", "assets/twoplayer.bmp"},
        {"backButton_BG", "assets/back.bmp"},
        {"playAgain", "assets/playAgain.bmp"},
        {"cit", "assets/cit.bmp"},
        {"cit_turn", "assets/cit_turn.bmp"},
        {"cit_win", "assets/cit_win.bmp"},
        {"coe", "assets/coe.bmp"},
        {"coe_turn", "assets/coe_turn.bmp"},
        {"coe_win", "assets/coe_win.bmp"},
    };
    for (const auto &asset : assets)
        textures[asset.key] = loadImage(renderer, asset.path);
    for (const auto &pair : textures)
    {
        if (!pair.second)
        {
            std::cerr << "Error: Failed to load texture '" << pair.first << "'!" << std::endl;
            return false;
        }
    }

    titleRect = {(SCREEN_WIDTH - 582) / 2, 90, 582, 96};
    catStandRect = {600, 450, 140, 100};
    catSitRect = {600, 450, 110, 110};
    twoPlayerRect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 + 60, 200, 80};
    onePlayerRect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, 200, 80};
    backRect = {20, 20, 40, 40};
    playAgainRect = {370, 490, 60, 60};
    citRect = {40, 250, 180, 90};
    coeRect = {570, 255, 180, 90};

    const SDL_Color black = {0, 0, 0, 255};
    const SDL_Color white = {255, 255, 255, 255};
    onePlayerButton = Button(onePlayerRect, white);
    twoPlayerButton = Button(twoPlayerRect, white);
    backButton = Button(backRect, black);
    playAgainButton = Button(playAgainRect, black);

    SdlBoardBuilder builder;
    board = BoardDirector(builder).makePlayfield(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    state = std::make_unique<HomepageState>();
    return true;
}

void Game::run()
{
    bool quit = false;
    SDL_Event e;
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
                quit = true;
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                if (auto next = state->onClick(*this, e.button.x, e.button.y))
                    state = std::move(next);
            }
        }
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);
        state->render(*this);
        SDL_RenderPresent(renderer);
        if (!vsync_)
            SDL_Delay(16); // ponytail: fixed 16ms sleep when the driver has no vsync
    }
}
