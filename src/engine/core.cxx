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
#include <json.hpp>
#include <fstream>
using json = nlohmann::json;

int core::screenWidth = 0;
int core::screenHeight = 0;
SDL_Texture* core::wallTexture = nullptr;
SDL_Surface* core::surfaceTexture =nullptr;

bool core::initiateWindow(const char* winTitle, int width, int height){
    screenWidth = width;
    screenHeight = height;
    return win.initiateWindow(winTitle,screenWidth,screenHeight);
}

void core::initiateGameLoop(){
    camera cam(screenWidth,screenHeight);
    addRequiredTextures();
    loadLevel("config/level1.json");
    //createSampleMap();
    addBackgroundLayersForParallax();
    inGameObject* player = objManager.getPlayerObject();
    if (player) {
        animation idleAnim(textureClass.getIndiviualTexture("Player_Idle"), 6, 0.12f, 68);
        idleAnim.setLooping(true);
        player->animationStaterManagerClass.addAnimation("idle", idleAnim);

        animation jumpAnim(textureClass.getIndiviualTexture("Player_Jump"), 8, 0.10f, 96);
        jumpAnim.setLooping(true);
        player->animationStaterManagerClass.addAnimation("jump", jumpAnim);

        animation runAnim(textureClass.getIndiviualTexture("Player_Run"), 6, 0.08f, 96);
        runAnim.setLooping(true);
        player->animationStaterManagerClass.addAnimation("run", runAnim);

        animation walkAnim(textureClass.getIndiviualTexture("Player_Walk"), 8, 0.10f, 65);
        walkAnim.setLooping(true);
        player->animationStaterManagerClass.addAnimation("walk", walkAnim);

        animation deathAnim(textureClass.getIndiviualTexture("Player_Dead"), 4, 0.15f, 96);
        deathAnim.setLooping(false);
        player->animationStaterManagerClass.addAnimation("death", deathAnim);
    }
    //objManager.setLevelWidth(6000);
    //objManager.setLevelHeight(3100);
    while(win.isRunning()){
        engineTime::startFrame();
        win.inputHandler();
        cam.updateCamera(player->getX(), player->getY(), objManager.getLevelWidth(),objManager.getLevelHeight());
        objManager.updateAllObjects();
        //std::cout << "Player X: " << player->getX() << ", Camera X: " << cam.getCameraX() << std::endl;
        background.update(engineTime::getDeltaTime());
        background.render(cam.getCameraX());

        objManager.renderAllObjects(win.getRenderer(),cam,textureClass);

        SDL_RenderPresent(win.getRenderer());
        engineTime::endFrame(144);
    }
}

SDL_Texture* core::getTexture(std::string objectTag){
    return textureClass.getIndiviualTexture(objectTag)->loadedTexture;
}

void core::createSampleMap() {
    objManager.addObject(0, 3000, 6000, 100, 0, mediumGrey, "Wall");

    for (int i = 0; i < 10; ++i) {
        objManager.addObject(200, 2900 - i * 200, 150, 20, 0, mediumGrey, "Wall");
    }

    objManager.addObject(1000, 2500, 200, 20, 0, mediumGrey, "Wall");
    objManager.addObject(1300, 2200, 200, 20, 0, mediumGrey, "Wall");
    objManager.addObject(1600, 1900, 200, 20, 0, mediumGrey, "Wall");
    objManager.addObject(1900, 1600, 200, 20, 0, mediumGrey, "Wall");
    objManager.addObject(2200, 1300, 200, 20, 0, mediumGrey, "Wall");

    for (int i = 0; i < 8; ++i) {
        objManager.addObject(4000 + i * 150, 1000 + i * 200, 150, 20, 0, mediumGrey, "Wall");
    }

    objManager.addObject(3000, 800, 200, 20, 0, mediumGrey, "Wall");
    objManager.addObject(3400, 800, 200, 20, 0, mediumGrey, "Wall");
    objManager.addObject(3800, 800, 200, 20, 0, mediumGrey, "Wall");

    objManager.addObject(5000, 700, 200, 20, 0, mediumGrey, "Wall");
    objManager.addObject(5000, 500, 200, 20, 0, mediumGrey, "Wall");
    objManager.addObject(5000, 300, 200, 20, 0, mediumGrey, "Wall");

    objManager.addObject(5500, 2900, 200, 20, 0, mediumGrey, "Wall");
    objManager.addObject(5700, 2700, 200, 20, 0, mediumGrey, "Wall");
    objManager.addObject(5900, 2500, 200, 20, 0, mediumGrey, "Wall");

    objManager.addObject(100, 100, 40, 80, 150, {0, 0, 0, 255}, "Player");
}

void core::loadLevel(const std::string& levelFile){
    std::ifstream file(levelFile);
    if(!file.is_open()){
        std::cout<<"Failed to load level file\n";
        return;
    }
    json data;
    file>>data;
    objManager.setLevelWidth(data["level"]["width"]);
    objManager.setLevelHeight(data["level"]["height"]);

    for(auto& wall:data["walls"]){
        objManager.addObject(wall["x"],wall["y"],
                            wall["width"],wall["height"],
                            wall["speed"],
                            mediumGrey,
                            wall["objectTag"]);
    }

    objManager.addObject(data["player"]["x"],data["player"]["y"],
                        data["player"]["width"],data["player"]["height"],
                        data["player"]["speed"],
                        {0, 0, 0, 255},
                        data["player"]["objectTag"]);
}

void core::addRequiredTextures(){
    SDL_Renderer* tempRenderer = win.getRenderer();
    std::ifstream file("config/textures.json");
    json data;
    file>>data;
    for(auto& textureJson: data["textures"]){
        std::string objectTag = textureJson["objectTag"];
        std::string filePath = textureJson["filePath"];
        textureClass.addTexture(tempRenderer,filePath,objectTag);
    }
}

void core::addBackgroundLayersForParallax(){
    parallaxManager temp(win.getRenderer(),screenWidth,screenHeight);
    background=temp;
    std::ifstream file("config/backgroundLayer.json");
    json data;
    file>>data;
    for(auto& backgroundLayer: data["backgroundLayer"]){
        std::string filePath = backgroundLayer["filePath"];
        float scrollSpeed = backgroundLayer["scrollSpeed"];
        int camDependenceX = backgroundLayer["camDependenceX"];
        background.addLayer(filePath,scrollSpeed,camDependenceX);
    }
}

int core::getScreenWidth(){return screenWidth;}
int core::getScreenHeight(){return screenHeight;}