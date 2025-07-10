#include <iostream>
#include <SDL2/SDL_render.h>
#include "window.hxx"

int main(int argc, char* argv[]){
    window win;
    if(!win.init("Shadow Swap",1280,720)) return -1;
    while(win.isRunning()){
        win.inputHandler();
        if(SDL_SetRenderDrawColor(win.getRenderer(),150,160,255,255)<0){
            std::cout<<"Render Draw Color failed, Error: "<<SDL_GetError()<<std::endl;
        }
        SDL_RenderClear(win.getRenderer());
        SDL_RenderPresent(win.getRenderer());
    }
    return 0;
}