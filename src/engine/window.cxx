#include "window.hxx"
#include <SDL2/SDL.h>

void window::init(){
    printf("Initializing SDL");
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)==-1){
        std::cout<<"Couldn't initalize SDL, Error: "<<SDL_GetError();
        exit(-1);
    }
    std::cout<<"SDL Initialized";
    SDL_Quit();
    exit(0);
}