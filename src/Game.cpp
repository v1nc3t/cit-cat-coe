#include "Game.h"

#include "BoardDirector.h"
#include "BoardDrawDirector.h"
#include "HomepageState.h"
#include "SdlBoardBuilder.h"
#include "SdlBoardDrawBuilder.h"

#include <algorithm>
#include <iostream>
#include <string>

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

TTF_Font *openFont(const char *const *paths, int count, int size)
{
    for (int i = 0; i < count; i++)
    {
        if (TTF_Font *font = TTF_OpenFont(paths[i], size))
            return font;
    }
    return nullptr;
}

SDL_Texture *makeText(SDL_Renderer *renderer, TTF_Font *font, const char *text)
{
    const SDL_Color white = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, white);
    if (!surface)
        return nullptr;
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void destroyScore(Game::ScoreLabel &label)
{
    if (label.texture)
        SDL_DestroyTexture(label.texture);
    label.texture = nullptr;
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
    destroyScore(citScoreLabel);
    destroyScore(coeScoreLabel);
    if (citWinText)
        SDL_DestroyTexture(citWinText);
    if (coeWinText)
        SDL_DestroyTexture(coeWinText);
    if (easyText)
        SDL_DestroyTexture(easyText);
    if (mediumText)
        SDL_DestroyTexture(mediumText);
    if (hardText)
        SDL_DestroyTexture(hardText);
    if (cotText)
        SDL_DestroyTexture(cotText);
    citWinText = nullptr;
    coeWinText = nullptr;
    easyText = nullptr;
    mediumText = nullptr;
    hardText = nullptr;
    cotText = nullptr;
    if (winFont_)
        TTF_CloseFont(winFont_);
    if (font_)
        TTF_CloseFont(font_);
    winFont_ = nullptr;
    font_ = nullptr;
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window_)
        SDL_DestroyWindow(window_);
    renderer = nullptr;
    window_ = nullptr;
    if (ttfReady_)
        TTF_Quit();
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

    if (TTF_Init() < 0)
    {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }
    ttfReady_ = true;
    const char *scoreFonts[] = {
        "assets/fonts/Roboto-Light.ttf",
        "/usr/share/fonts/truetype/roboto/unhinted/RobotoTTF/Roboto-Light.ttf",
        "C:/Windows/Fonts/Roboto-Light.ttf",
    };
    const char *winFonts[] = {
        "assets/fonts/RobotoMono-Light.ttf",
        "/usr/share/fonts/truetype/roboto/unhinted/RobotoTTF/RobotoMono-Light.ttf",
        "C:/Windows/Fonts/RobotoMono-Light.ttf",
    };
    font_ = openFont(scoreFonts, sizeof(scoreFonts) / sizeof(scoreFonts[0]), 42);
    winFont_ = openFont(winFonts, sizeof(winFonts) / sizeof(winFonts[0]), 28);
    if (!font_ || !winFont_)
    {
        std::cerr << "Could not open Roboto or the typewriter font in assets/fonts." << std::endl;
        return false;
    }

    window_ = SDL_CreateWindow("cit cat coe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
        {"backButton_BG", "assets/back.bmp"},
        {"playAgain", "assets/playAgain.bmp"},
        {"cit", "assets/cit.bmp"},
        {"cit_turn", "assets/cit_turn.bmp"},
        {"coe", "assets/coe.bmp"},
        {"coe_turn", "assets/coe_turn.bmp"},
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

    citWinText = makeText(renderer, winFont_, "cit wins");
    coeWinText = makeText(renderer, winFont_, "coe wins");
    easyText = makeText(renderer, font_, "easy");
    mediumText = makeText(renderer, font_, "medium");
    hardText = makeText(renderer, font_, "hard");
    cotText = makeText(renderer, font_, "cot");
    if (!citWinText || !coeWinText || !easyText || !mediumText || !hardText || !cotText)
    {
        std::cerr << "Could not render win text. TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }

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

void Game::renderScore(ScoreLabel &label, int score, const SDL_Rect &image)
{
    if (score != label.shown)
    {
        destroyScore(label);
        const SDL_Color white = {255, 255, 255, 255};
        SDL_Surface *surface = TTF_RenderText_Blended(font_, std::to_string(score).c_str(), white);
        if (surface)
        {
            label.texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
        }
        label.shown = score;
    }
    if (!label.texture)
        return;
    int width = 0;
    int height = 0;
    SDL_QueryTexture(label.texture, nullptr, nullptr, &width, &height);
    SDL_Rect dest = {image.x + (image.w - width) / 2, image.y + image.h + 12, width, height};
    SDL_RenderCopy(renderer, label.texture, nullptr, &dest);
}

void Game::renderFitted(SDL_Texture *texture, const SDL_Rect &area)
{
    if (!texture)
        return;
    int width = 0;
    int height = 0;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    if (width <= 0 || height <= 0)
        return;
    const float scale = std::min((area.w - 8) / static_cast<float>(width), (area.h - 8) / static_cast<float>(height));
    const int destW = static_cast<int>(width * scale);
    const int destH = static_cast<int>(height * scale);
    const SDL_Rect dest = {area.x + (area.w - destW) / 2, area.y + (area.h - destH) / 2, destW, destH};
    SDL_RenderCopy(renderer, texture, nullptr, &dest);
}

void Game::renderMatch()
{
    SdlBoardDrawBuilder drawBuilder(board, player);
    BoardDrawDirector().draw(drawBuilder);

    SDL_RenderCopy(renderer, textures["backButton_BG"], nullptr, &backRect);
    backButton.renderButton(renderer);
    SDL_RenderCopy(renderer, textures["cat_sit"], nullptr, &catSitRect);

    if (board.isFull())
    {
        SDL_RenderCopy(renderer, textures["playAgain"], nullptr, &playAgainRect);
        playAgainButton.renderButton(renderer);
        SDL_RenderCopy(renderer, textures["cit"], nullptr, &citRect);
        SDL_RenderCopy(renderer, textures["coe"], nullptr, &coeRect);
    }
    else if (player.winner == MARK_NONE)
    {
        SDL_RenderCopy(renderer, textures[player.mark == MARK_O ? "cit_turn" : "cit"], nullptr, &citRect);
        SDL_RenderCopy(renderer, textures[player.mark == MARK_O ? "coe" : "coe_turn"], nullptr, &coeRect);
    }
    else
    {
        SDL_RenderCopy(renderer, textures["playAgain"], nullptr, &playAgainRect);
        playAgainButton.renderButton(renderer);
        if (player.winner == MARK_O)
            renderFitted(citWinText, citRect);
        else
            SDL_RenderCopy(renderer, textures["cit"], nullptr, &citRect);
        if (player.winner == MARK_X)
            renderFitted(coeWinText, coeRect);
        else
            SDL_RenderCopy(renderer, textures["coe"], nullptr, &coeRect);
    }
    renderScore(citScoreLabel, citScore, citRect);
    renderScore(coeScoreLabel, coeScore, coeRect);
}

bool Game::placeMark(int row, int col)
{
    if (player.winner != MARK_NONE || row < 0 || col < 0 || row > 2 || col > 2)
        return false;
    if (!board.fillCell(row, col, player.mark))
        return false;
    if (board.checkWin(player))
    {
        player.setWinner();
        if (player.winner == MARK_O)
            citScore++;
        else
            coeScore++;
    }
    player.switchPlayer();
    return true;
}
