#include "core.hxx"
#include "inGameObject.hxx"
#include "time.hxx"
#include "parallaxManager.hxx"
#include "animation.hxx"
#include "objectManager.hxx"
#include "texture.hxx"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

SDL_Texture* core::wallTexture = nullptr;
SDL_Texture* core::backgroundImage=nullptr;
SDL_Surface* core::surfaceTexture =nullptr;
SDL_Texture* core::playerTexture=nullptr;

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
    texture* playerTexture = new texture();
    texture* playerTexture1 = new texture();
    texture* playerTexture2 = new texture();
    texture* playerTexture3 = new texture();
    texture* playerTexture4 = new texture();
    createSampleMap();
    parallaxManager background(win.getRenderer(),screenWidth,screenHeight);
    background.addLayer("assets/backgrounds/Animated_Backgrounds/Parallax/Bg_1/sky.png",0.0);
    background.addLayer("assets/backgrounds/Animated_Backgrounds/Parallax/Bg_1/rocks_1.png",0.0);
    background.addLayer("assets/backgrounds/Animated_Backgrounds/Parallax/Bg_1/rocks_2.png",0.0);
    background.addLayer("assets/backgrounds/Animated_Backgrounds/Parallax/Bg_1/long_cloud1920x1080.png",3.0);
    inGameObject* player = objManager.getObjectByTag("Player");
    if (player) {
        playerTexture->loadTextureFromFile(win.getRenderer(),"assets/Character/Ninja_Peasant/Idle.png");
        player->animationStaterManagerClass.addAnimation("idle", animation(playerTexture, 6, 0.12f, 68));
        playerTexture1->loadTextureFromFile(win.getRenderer(),"assets/Character/Ninja_Peasant/Jump.png");
        player->animationStaterManagerClass.addAnimation("jump", animation(playerTexture1, 8, 0.10f, 96));
        playerTexture2->loadTextureFromFile(win.getRenderer(),"assets/Character/Ninja_Peasant/Run.png");
        player->animationStaterManagerClass.addAnimation("run",  animation(playerTexture2, 6, 0.08f, 96));
        playerTexture3->loadTextureFromFile(win.getRenderer(),"assets/Character/Ninja_Peasant/Walk.png");
        player->animationStaterManagerClass.addAnimation("walk", animation(playerTexture3, 8, 0.10f, 65));
        playerTexture4->loadTextureFromFile(win.getRenderer(),"assets/Character/Ninja_Peasant/Dead.png");
        player->animationStaterManagerClass.addAnimation("death", animation(playerTexture4, 4, 0.15f, 96));
    }
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
    objManager.addObject(0,670,1280,50,0,mediumGrey,"Wall");
    objManager.addObject(0,240,40,40,150,{0,0,0,255},"Player");
}