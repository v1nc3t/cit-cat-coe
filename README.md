# Cit Cat Coe

Cat-themed Tic Tac Toe. Cit plays O and goes first. Coe plays X. Two players share one machine. The one-player button is on the menu, and that mode is not implemented yet.

## Installation

`make` picks the platform. Same sources on Windows and Linux.

Windows (64-bit MinGW). SDL2 is already in `src/include` and `src/lib`.

1. Install [MSYS2](https://www.msys2.org/), then a 64-bit toolchain:

```
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-SDL2_ttf make
```

2. From the project root:

```
make
```

That produces `CitCatCoe.exe`. The exe icon comes from `resources.rc`. `make` rebuilds `resources.o` with `windres` when that file changes.

Linux. SDL2 comes from the system, not the MinGW copy in `src/lib`.

```
sudo apt install g++ make libsdl2-dev libsdl2-ttf-dev pkg-config
make
```

That produces `CitCatCoe`.

## How to run

Start the program from the project root. Image paths are `assets/`.

Windows:

```
./CitCatCoe.exe
```

Linux:

```
./CitCatCoe
```

On the title screen, choose a mode.

- **Two player:** click an empty cell. Cit is O, Coe is X. A line marks a win, and that player's score under the portrait goes up by one. A full board with no three-in-a-row is a draw. Play again clears the board and keeps the score. Back returns to the title screen. The next match starts at 0.
- **One player:** pick Easy, Medium, or Hard. You play Cit (O) and go first. Coe is the bot. Easy plays a random empty cell. Medium plays well, and about one move in three is random. Hard does not give the game away. Play again keeps the score and the difficulty. Back returns to the title screen.

## Description

The window is 800 by 600. The playfield is a 300 by 300 grid in the center. Cells are integers:

| Value | Meaning |
| --- | --- |
| 0 | O (Cit) |
| 1 | X (Coe) |
| -1 | Empty |

A win is three of the same mark in a row, column, or either diagonal. The current player is shown with the `*_turn` images. The winner's portrait is replaced by the text cit wins or coe wins, set in Roboto Mono Light. Scores and the difficulty labels use Roboto Light. Both files are in `assets/fonts/`.

Before the window opens, `main` checks those rules (wins, a draw, and a filled cell). If that check fails, the program exits and prints `board rules check failed`.

## Dev

C++17 and SDL2. Headers are in `include/`. Sources are in `src/`. Art is in `assets/`.

| Piece | Role |
| --- | --- |
| `src/main.cpp` | Rules check, then start the game |
| `src/Game.cpp` | Window, textures, main loop |
| `include/Board.h` | 3x3 grid, moves, win checks |
| `SdlBoardBuilder` / `BoardDirector` | Build the playfield on an SDL renderer |
| `SdlBoardDrawBuilder` / `BoardDrawDirector` | Draw background, grid, marks, win line |
| `HomepageState`, `TwoPlayerState`, `OnePlayerState` | Screen behavior |

`PlayState` is the shared click handling for both game screens (place a mark, play again, back). `src/Bot.cpp` chooses Coe's move.

Rebuild the Windows icon resource after editing `resources.rc`:

```
windres resources.rc -o resources.o
```
