#include "core.hxx"
#include "inGameObject.hxx"
#include "time.hxx"
#include "parallaxManager.hxx"
#include "animation.hxx"
#include "objectManager.hxx"
#include "texture.hxx"
#include "camera.hxx"
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
    camera cam(screenWidth,screenHeight);
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
    background.addLayer("assets/backgrounds/Animated_Backgrounds/Parallax/Night_forest/sky.png",0.8,1);
    background.addLayer("assets/backgrounds/Animated_Backgrounds/Parallax/Night_forest/rocks.png",0.7,1);
    background.addLayer("assets/backgrounds/Animated_Backgrounds/Parallax/Night_forest/clouds_1.png",2.0,0);
    background.addLayer("assets/backgrounds/Animated_Backgrounds/Parallax/Night_forest/clouds_2.png",2.0,0);
    background.addLayer("assets/backgrounds/Animated_Backgrounds/Parallax/Night_forest/ground_1.png",0.7,1);
    background.addLayer("assets/backgrounds/Animated_Backgrounds/Parallax/Night_forest/ground_2.png",0.6,1);
    background.addLayer("assets/backgrounds/Animated_Backgrounds/Parallax/Night_forest/ground_3.png",0.5,1);
    background.addLayer("assets/backgrounds/Animated_Backgrounds/Parallax/Night_forest/plant.png",0.1,1);
    /*background.addLayer("assets/backgrounds/Animated_Backgrounds/Parallax/Basic_Scene/_08_clouds.png",6.0);
    background.addLayer("assets/backgrounds/Animated_Backgrounds/Parallax/Basic_Scene/_09_distant_clouds1.png",4.0);
    background.addLayer("assets/backgrounds/Animated_Backgrounds/Parallax/Basic_Scene/_10_distant_clouds.png",3.0);*/
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
        cam.updateCamera(player->getX(), player->getY(),6000,3100);
        objManager.updateAllObjects();
        //std::cout << "Player X: " << player->getX() << ", Camera X: " << cam.getCameraX() << std::endl;
        background.update(engineTime::getDeltaTime());
        background.render(cam.getCameraX());

        objManager.renderAllObjects(win.getRenderer(),cam);

        SDL_RenderPresent(win.getRenderer());
        engineTime::endFrame(144);
    }
}

SDL_Texture* core::getTexture(std::string objectTag){
    if(objectTag=="Wall") return core::wallTexture;
    else return nullptr;
}

void core::createSampleMap() {
    // Ground (entire horizontal stretch)
    objManager.addObject(0, 3000, 6000, 100, 0, mediumGrey, "Wall");

    // Left Climb: vertical platform stairway
    for (int i = 0; i < 10; ++i) {
        objManager.addObject(200, 2900 - i * 200, 150, 20, 0, mediumGrey, "Wall");
    }

    // Central floating islands
    objManager.addObject(1000, 2500, 200, 20, 0, mediumGrey, "Wall");
    objManager.addObject(1300, 2200, 200, 20, 0, mediumGrey, "Wall");
    objManager.addObject(1600, 1900, 200, 20, 0, mediumGrey, "Wall");
    objManager.addObject(1900, 1600, 200, 20, 0, mediumGrey, "Wall");
    objManager.addObject(2200, 1300, 200, 20, 0, mediumGrey, "Wall");

    // Right-side descending path
    for (int i = 0; i < 8; ++i) {
        objManager.addObject(4000 + i * 150, 1000 + i * 200, 150, 20, 0, mediumGrey, "Wall");
    }

    // Wide mid-air jump challenges
    objManager.addObject(3000, 800, 200, 20, 0, mediumGrey, "Wall");
    objManager.addObject(3400, 800, 200, 20, 0, mediumGrey, "Wall");
    objManager.addObject(3800, 800, 200, 20, 0, mediumGrey, "Wall");

    // Mid-air vertical corridor platforms
    objManager.addObject(5000, 700, 200, 20, 0, mediumGrey, "Wall");
    objManager.addObject(5000, 500, 200, 20, 0, mediumGrey, "Wall");
    objManager.addObject(5000, 300, 200, 20, 0, mediumGrey, "Wall");

    // Bottom-right platform cluster
    objManager.addObject(5500, 2900, 200, 20, 0, mediumGrey, "Wall");
    objManager.addObject(5700, 2700, 200, 20, 0, mediumGrey, "Wall");
    objManager.addObject(5900, 2500, 200, 20, 0, mediumGrey, "Wall");

    // Player spawn — top-left corner
    objManager.addObject(100, 100, 40, 80, 150, {0, 0, 0, 255}, "Player");
}

int core::getScreenWidth(){return screenWidth;}
int core::getScreenHeight(){return screenHeight;}