# Cit Cat Coe

Cat-themed Tic Tac Toe. Cit plays O and goes first. Coe plays X. Two players share one machine. The one-player button is on the menu, and that mode is not implemented yet.

## Installation

`make` picks the platform. Same sources on Windows and Linux.

Windows (64-bit MinGW). SDL2 is already in `src/include` and `src/lib`.

1. Install [MSYS2](https://www.msys2.org/), then a 64-bit toolchain:

```
pacman -S mingw-w64-x86_64-gcc make
```

2. From the project root:

```
make
```

That produces `CitCatCoe.exe`. The exe icon comes from `resources.rc`. `make` rebuilds `resources.o` with `windres` when that file changes.

Linux. SDL2 comes from the system, not the MinGW copy in `src/lib`.

```
sudo apt install g++ make libsdl2-dev pkg-config
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

- **Two player:** click an empty cell. Cit is O, Coe is X. A line marks a win. A full board with no three-in-a-row is a draw. Play again resets the board. Back returns to the title screen.
- **One player:** the screen stays empty. Use the window close button to quit.

## Description

The window is 800 by 600. The playfield is a 300 by 300 grid in the center. Cells are integers:

| Value | Meaning |
| --- | --- |
| 0 | O (Cit) |
| 1 | X (Coe) |
| -1 | Empty |

A win is three of the same mark in a row, column, or either diagonal. The current player is shown with the `*_turn` images. The winner is shown with the `*_win` image.

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

`PlayState` is the shared click handling for both game screens (place a mark, play again, back).

One-player rendering is `OnePlayerState::render`. Put that mode there.

Rebuild the Windows icon resource after editing `resources.rc`:

```
windres resources.rc -o resources.o
```
