#include "core.hxx"
#include "inGameObject.hxx"
#include <SDL.h>
#include <iostream>

bool core::initiateWindow(const char* winTitle, int width, int height){
    return win.initiateWindow(winTitle,width,height);
}

void core::initiateGameLoop(){
    inGameObject object_Rect(100,100,50);   
    while(win.isRunning()){
        win.inputHandler();
        object_Rect.updateObjectState();
        if(SDL_SetRenderDrawColor(win.getRenderer(),150,160,255,255)<0){
            std::cout<<"Render Draw Color failed, Error: "<<SDL_GetError()<<std::endl;
        }
        SDL_RenderClear(win.getRenderer());
        object_Rect.renderObject(win.getRenderer());
        SDL_RenderPresent(win.getRenderer());
    }
}