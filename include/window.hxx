#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <string>
#include "renderer.hxx"

class window{
    private:
    SDL_Window* win = NULL;
    Renderer* renderer = NULL;
    bool appRunning = false;

    public:
    bool initiateWindow(const char*,int,int);
    void inputHandler();
    bool isRunning() const;
    Renderer* getRenderer();
    ~window();
};