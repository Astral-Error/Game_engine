#include <iostream>
#include <SDL_render.h>
#include "window.hxx"
#include "core.hxx"

int main(int argc, char* argv[]){
    if(SDL_Init(SDL_INIT_VIDEO)<0){
        std::cout<<"SDL Init failed\n";
        return false;
    }
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0,&displayMode);
    std::cout<<displayMode.w<<std::endl<<displayMode.h;
    core engine;
    if(!engine.initiateWindow("Shadow Swap",displayMode.w,displayMode.h)) return -1;
    engine.initiateGameLoop();
    return 0;
}