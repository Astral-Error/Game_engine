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

int core::screenWidth = 0;
int core::screenHeight = 0;
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
    surfaceTexture = IMG_Load("assets/textures/WallTexture/greyBrickTexture.png");
    if (!surfaceTexture) {
        std::cout << "IMG_Load failed: " << IMG_GetError() << std::endl;
    }
    wallTexture = SDL_CreateTextureFromSurface(win.getRenderer(),surfaceTexture);
    if (!wallTexture) {
        std::cout << "CreateTexture failed: " << SDL_GetError() << std::endl;
    }
    SDL_FreeSurface(surfaceTexture);
    texture* playerTexture = new texture(win.getRenderer(),"assets/Character/Ninja_Peasant/Idle.png");
    texture* playerTexture1 = new texture(win.getRenderer(),"assets/Character/Ninja_Peasant/Jump.png");
    texture* playerTexture2 = new texture(win.getRenderer(),"assets/Character/Ninja_Peasant/Run.png");
    texture* playerTexture3 = new texture(win.getRenderer(),"assets/Character/Ninja_Peasant/Walk.png");
    texture* playerTexture4 = new texture(win.getRenderer(),"assets/Character/Ninja_Peasant/Dead.png");
    createSampleMap();
    parallaxManager background(win.getRenderer(),screenWidth,screenHeight);
    background.addLayer("assets/backgrounds/Animated_Backgrounds/Parallax/Forest/forest_sky.png",1.0);
    background.addLayer("assets/backgrounds/Animated_Backgrounds/Parallax/Forest/forest_mountain.png",0.0);
    background.addLayer("assets/backgrounds/Animated_Backgrounds/Parallax/Forest/forest_back.png",0.0);
    background.addLayer("assets/backgrounds/Animated_Backgrounds/Parallax/Forest/forest_mid.png",0.0);
    background.addLayer("assets/backgrounds/Animated_Backgrounds/Parallax/Forest/forest_short.png",0.0);
    background.addLayer("assets/backgrounds/Animated_Backgrounds/Parallax/Basic_Scene/_08_clouds.png",6.0);
    background.addLayer("assets/backgrounds/Animated_Backgrounds/Parallax/Basic_Scene/_09_distant_clouds1.png",4.0);
    background.addLayer("assets/backgrounds/Animated_Backgrounds/Parallax/Basic_Scene/_10_distant_clouds.png",3.0);
    inGameObject* player = objManager.getObjectByTag("Player");
    if (player) {
        // Idle (loop)
        animation idleAnim(playerTexture, 6, 0.12f, 68);
        idleAnim.setLooping(true);
        player->animationStaterManagerClass.addAnimation("idle", idleAnim);

        // Jump (not looping)
        animation jumpAnim(playerTexture1, 8, 0.10f, 96);
        jumpAnim.setLooping(true); // or skip this line
        player->animationStaterManagerClass.addAnimation("jump", jumpAnim);

        // Run (loop)
        animation runAnim(playerTexture2, 6, 0.08f, 96);
        runAnim.setLooping(true);
        player->animationStaterManagerClass.addAnimation("run", runAnim);

        // Walk (loop)
        animation walkAnim(playerTexture3, 8, 0.10f, 65);
        walkAnim.setLooping(true);
        player->animationStaterManagerClass.addAnimation("walk", walkAnim);

        // Death (not looping)
        animation deathAnim(playerTexture4, 4, 0.15f, 96);
        deathAnim.setLooping(false);
        player->animationStaterManagerClass.addAnimation("death", deathAnim);
    }
    while(win.isRunning()){
        engineTime::startFrame();
        win.inputHandler();

        objManager.updateAllObjects();
        background.update(engineTime::getDeltaTime());
        background.render();

        objManager.renderAllObjects(win.getRenderer());

        SDL_RenderPresent(win.getRenderer());
        engineTime::endFrame(144);
    }
}

SDL_Texture* core::getTexture(std::string objectTag){
    if(objectTag=="Wall") return core::wallTexture;
    else return nullptr;
}

void core::createSampleMap(){
    objManager.addObject(0,450,100,20,0,mediumGrey,"Wall");
    objManager.addObject(100,650,100,20,0,mediumGrey,"Wall");
    objManager.addObject(300,550,100,20,0,mediumGrey,"Wall");
    objManager.addObject(400,450,150,20,0,mediumGrey,"Wall");
    objManager.addObject(700,350,150,20,0,mediumGrey,"Wall");
    objManager.addObject(850,250,150,20,0,mediumGrey,"Wall");
    objManager.addObject(1000,350,150,20,0,mediumGrey,"Wall");
    objManager.addObject(0,1030,1920,50,0,mediumGrey,"Wall");
    objManager.addObject(0,240,40,80,150,{0,0,0,255},"Player");
}

int core::getScreenWidth(){return screenWidth;}
int core::getScreenHeight(){return screenHeight;}