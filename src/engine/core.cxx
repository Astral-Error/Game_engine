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

        if(SDL_SetRenderDrawColor(win.getRenderer(),105,105,105,255)<0){
            std::cout<<"Render Draw Color failed, Error: "<<SDL_GetError()<<std::endl;
        }

        SDL_RenderClear(win.getRenderer());

        objManager.renderAllObjects(win.getRenderer());

        SDL_RenderPresent(win.getRenderer());
        engineTime::endFrame(60);
    }
}

void core::createSampleMap(){
    objManager.addObject(0,300,100,20,0,mediumGrey,"Wall");
    objManager.addObject(100,500,100,20,0,mediumGrey,"Wall");
    objManager.addObject(300,400,100,20,0,mediumGrey,"Wall");
    objManager.addObject(400,300,150,20,0,mediumGrey,"Wall");
    objManager.addObject(700,200,150,20,0,mediumGrey,"Wall");
    objManager.addObject(850,100,150,20,0,mediumGrey,"Wall");
    objManager.addObject(1000,200,150,20,0,mediumGrey,"Wall");
    objManager.addObject(0,240,40,40,150,white,"Player");
}