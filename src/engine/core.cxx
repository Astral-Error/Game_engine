#include "core.hxx"
#include "inGameObject.hxx"
#include "time.hxx"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

SDL_Texture* core::wallTexture = nullptr;
SDL_Texture* core::backgroundImage=nullptr;
SDL_Surface* core::surfaceTexture =nullptr;

bool core::initiateWindow(const char* winTitle, int width, int height){
    return win.initiateWindow(winTitle,width,height);
}

void core::initiateGameLoop(){
    surfaceTexture = IMG_Load("assets/textures/WallTexture/ClassicBrickWall-rect.png");
    if (!surfaceTexture) {
        std::cout << "IMG_Load failed: " << IMG_GetError() << std::endl;
    }
    wallTexture = SDL_CreateTextureFromSurface(win.getRenderer(),surfaceTexture);
    if (!wallTexture) {
        std::cout << "CreateTexture failed: " << SDL_GetError() << std::endl;
    }
    SDL_FreeSurface(surfaceTexture);
    createSampleMap();
    surfaceTexture = IMG_Load("assets/backgrounds/CloudySkiesOverTheMountain.png");
    if (!surfaceTexture) {
        std::cout << "IMG_Load failed: " << IMG_GetError() << std::endl;
    }
    backgroundImage = SDL_CreateTextureFromSurface(win.getRenderer(),surfaceTexture);
    if (!backgroundImage) {
        std::cout << "CreateTexture failed: " << SDL_GetError() << std::endl;
    }
    SDL_FreeSurface(surfaceTexture);
    while(win.isRunning()){
        engineTime::startFrame();
        win.inputHandler();

        objManager.updateAllObjects();
        SDL_Rect screenBackground={0,0,1280,720};
        SDL_RenderCopy(win.getRenderer(),backgroundImage,nullptr,&screenBackground);

        objManager.renderAllObjects(win.getRenderer());

        SDL_RenderPresent(win.getRenderer());
        engineTime::endFrame(60);
    }
}

SDL_Texture* core::getTexture(std::string objectTag){
    
    if(objectTag=="Wall") return core::wallTexture;
    else return nullptr;
}

void core::createSampleMap(){
    objManager.addObject(0,300,100,20,0,mediumGrey,"Wall");
    objManager.addObject(100,500,100,20,0,mediumGrey,"Wall");
    objManager.addObject(300,400,100,20,0,mediumGrey,"Wall");
    objManager.addObject(400,300,150,20,0,mediumGrey,"Wall");
    objManager.addObject(700,200,150,20,0,mediumGrey,"Wall");
    objManager.addObject(850,100,150,20,0,mediumGrey,"Wall");
    objManager.addObject(1000,200,150,20,0,mediumGrey,"Wall");
    objManager.addObject(0,240,40,40,150,{0,0,0,255},"Player");
}