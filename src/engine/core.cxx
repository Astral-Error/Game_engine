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
    createSampleMap();
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

void core::createSampleMap(){
    objManager.addObject(0,300,100,20,0,"Wall 1");
    objManager.addObject(100,500,100,20,0,"Wall 2");
    objManager.addObject(300,400,100,20,0,"Wall 3");
    objManager.addObject(400,300,150,20,0,"Wall 4");
    objManager.addObject(700,200,150,20,0,"Wall 5");
    objManager.addObject(850,100,150,20,0,"Wall 6");
    objManager.addObject(1000,200,150,20,0,"Wall 7");
    objManager.addObject(640,360,40,40,150,"Player");
}