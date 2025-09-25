#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <string>

class window{
    private:
    SDL_Window* win = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_GLContext glContext = NULL;
    bool appRunning = false;

    public:
    bool initiateWindow(const char*,int,int);
    void inputHandler();
    bool isRunning() const;
    SDL_Renderer* getRenderer();
    SDL_GLContext getGLContext();
    ~window();
};