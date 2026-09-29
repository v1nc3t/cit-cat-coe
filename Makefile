CXX = g++
SRCS = \
	src/main.cpp \
	src/Game.cpp \
	src/Button.cpp \
	src/Player.cpp \
	src/Board.cpp \
	src/SdlBoardBuilder.cpp \
	src/BoardDirector.cpp \
	src/SdlBoardDrawBuilder.cpp \
	src/BoardDrawDirector.cpp \
	src/HomepageState.cpp \
	src/PlayState.cpp \
	src/TwoPlayerState.cpp \
	src/OnePlayerState.cpp

ifeq ($(OS),Windows_NT)
PLATFORM := windows
else
UNAME_S := $(shell uname -s)
ifneq (,$(findstring MINGW,$(UNAME_S)))
PLATFORM := windows
else ifneq (,$(findstring MSYS,$(UNAME_S)))
PLATFORM := windows
else
PLATFORM := linux
endif
endif

ifeq ($(PLATFORM),windows)
TARGET := CitCatCoe.exe
CXXFLAGS := -std=c++17 -I include -I src/include
LDFLAGS := resources.o -L src/lib -lmingw32 -lSDL2main -lSDL2_ttf -lSDL2 -mwindows
else
TARGET := CitCatCoe
SDL2_CFLAGS := $(shell pkg-config --cflags sdl2 SDL2_ttf 2>/dev/null)
SDL2_LIBS := $(shell pkg-config --libs sdl2 SDL2_ttf 2>/dev/null)
ifeq ($(SDL2_LIBS),)
$(error SDL2 or SDL2_ttf not found. Install libsdl2-dev, libsdl2-ttf-dev, and pkg-config)
endif
CXXFLAGS := -std=c++17 -I include $(SDL2_CFLAGS)
LDFLAGS := $(SDL2_LIBS)
endif

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS) $(LDFLAGS)

ifeq ($(PLATFORM),windows)
$(TARGET): resources.o

resources.o: resources.rc
	windres $< -o $@
endif

clean:
	rm -f CitCatCoe CitCatCoe.exe
