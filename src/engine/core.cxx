#include "core.hxx"
#include "inGameObject.hxx"
#include "time.hxx"
#include <SDL.h>
#include <iostream>
#include <vector>

bool core::initiateWindow(const char* winTitle, int width, int height){
    return win.initiateWindow(winTitle,width,height);
}

void core::initiateGameLoop(){
    objManager.addObject(100,100,50,"Square");
    while(win.isRunning()){
        engineTime::startFrame();
        win.inputHandler();

        objManager.updateAllObjects();

        if(SDL_SetRenderDrawColor(win.getRenderer(),150,160,255,255)<0){
            std::cout<<"Render Draw Color failed, Error: "<<SDL_GetError()<<std::endl;
        }

        SDL_RenderClear(win.getRenderer());

        objManager.renderAllObjects(win.getRenderer());

        SDL_RenderPresent(win.getRenderer());
        engineTime::endFrame(60);
    }
}