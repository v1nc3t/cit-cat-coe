#pragma once

#include "Board.h"
#include "Button.h"
#include "GameState.h"
#include "Player.h"

#include "Sdl.h"
#include "SdlTtf.h"

#include <map>
#include <memory>
#include <string>

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

class Game {
    SDL_Window *window_ = nullptr;
    TTF_Font *font_ = nullptr;
    TTF_Font *winFont_ = nullptr;
    bool sdlReady_ = false;
    bool ttfReady_ = false;
    bool vsync_ = false;

public:
    struct ScoreLabel {
        SDL_Texture *texture = nullptr;
        int shown = -1;
    };

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
    int citScore = 0;
    int coeScore = 0;
    ScoreLabel citScoreLabel;
    ScoreLabel coeScoreLabel;
    SDL_Texture *citWinText = nullptr;
    SDL_Texture *coeWinText = nullptr;
    SDL_Texture *easyText = nullptr;
    SDL_Texture *mediumText = nullptr;
    SDL_Texture *hardText = nullptr;
    SDL_Texture *cotText = nullptr;
    std::unique_ptr<GameState> state;

    ~Game();
    bool init();
    void run();
    void renderScore(ScoreLabel &label, int score, const SDL_Rect &image);
    void renderFitted(SDL_Texture *texture, const SDL_Rect &area);
    void renderMatch();
    bool placeMark(int row, int col);
};
