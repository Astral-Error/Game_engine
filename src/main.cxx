#include <iostream>
#include <SDL2/SDL_render.h>
#include "window.hxx"
#include "core.hxx"

int main(int argc, char* argv[]){
    core engine;
    if(!engine.init("Shadow Swap",1280,720)) return -1;
    engine.initiateGameLoop();
    return 0;
}

//testing webhooks