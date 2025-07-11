#include "core.hxx"
#include <SDL.h>
#include <iostream>

bool core::init(const char* winTitle, int width, int height){
    return win.init(winTitle,width,height);
}

void core::initiateGameLoop(){
    while(win.isRunning()){
        win.inputHandler();
        if(SDL_SetRenderDrawColor(win.getRenderer(),150,160,255,255)<0){
            std::cout<<"Render Draw Color failed, Error: "<<SDL_GetError()<<std::endl;
        }
        SDL_RenderClear(win.getRenderer());
        SDL_RenderPresent(win.getRenderer());
    }
}