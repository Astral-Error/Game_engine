#include <iostream>
#include <SDL2/SDL.h>

class window{
    private:
    SDL_Window* win = NULL;

    public:
    void init();
    void swapBuffers();
    void Shutdown();
};