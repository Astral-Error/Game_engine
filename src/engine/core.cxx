#include "core.hxx"
#include "inGameObject.hxx"
#include "time.hxx"
#include "parallaxManager.hxx"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

SDL_Texture* core::wallTexture = nullptr;
SDL_Texture* core::backgroundImage=nullptr;
SDL_Surface* core::surfaceTexture =nullptr;

bool core::initiateWindow(const char* winTitle, int width, int height){
    screenWidth = width;
    screenHeight = height;
    return win.initiateWindow(winTitle,screenWidth,screenHeight);
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
    parallaxManager background(win.getRenderer(),screenWidth,screenHeight);
    background.addLayer("assets/backgrounds/Animated_Backgrounds/Parallax/Bg_1/sky.png",0.0);
    background.addLayer("assets/backgrounds/Animated_Backgrounds/Parallax/Bg_1/rocks_1.png",0.0);
    background.addLayer("assets/backgrounds/Animated_Backgrounds/Parallax/Bg_1/rocks_2.png",0.0);
    background.addLayer("assets/backgrounds/Animated_Backgrounds/Parallax/Bg_1/long_cloud1920x1080.png",3.0);
    while(win.isRunning()){
        engineTime::startFrame();
        win.inputHandler();

        objManager.updateAllObjects();
        background.update(engineTime::getDeltaTime());
        background.render();

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