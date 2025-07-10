#pragma once
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <string>

class window{
    private:
    SDL_Window* win = NULL;
    SDL_Renderer* renderer = NULL;
    bool appRunning = false;

    public:
    bool init(const char*,int,int);
    void inputHandler();
    bool isRunning() const;
    ~window();
};