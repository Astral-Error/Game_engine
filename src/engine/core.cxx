#include "core.hxx"
#include "inGameObject.hxx"
#include "time.hxx"
#include "parallaxManager.hxx"
#include "animation.hxx"
#include "objectManager.hxx"
#include "texture.hxx"
#include "camera.hxx"
#include "sceneManager.hxx"
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
    sceneMgr = new sceneManager(this);
    sceneMgr->getScenes("config/sceneManager.json");
    sceneMgr->loadScene(0);
    inGameObject* player = objManager.getPlayerObject();
    if (player) getAnimationForPlayer(player);
    while(win.isRunning()){
        engineTime::startFrame();
        win.inputHandler();
        cam.updateCamera(player->getX(), player->getY(), objManager.getLevelWidth(),objManager.getLevelHeight());
        objManager.updateAllObjects();
        if (player->getX() + player->getWidth() > objManager.getLevelWidth() - 50) {
            sceneMgr->loadNextScene();

            player = objManager.getPlayerObject();
            if (player) getAnimationForPlayer(player);
            if (player) {
                player->setX(0);
                player->setY(2900);
                getAnimationForPlayer(player);
            }
        }
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

void core::loadLevel(const std::string& levelFile){
    objManager.clearAllObjects();
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

void core::addBackgroundLayersForParallax(const std::string& bgFile){
    parallaxManager temp(win.getRenderer(),screenWidth,screenHeight);
    background=temp;
    std::ifstream file(bgFile);
    json data;
    file>>data;
    for(auto& backgroundLayer: data["backgroundLayer"]){
        std::string filePath = backgroundLayer["filePath"];
        float scrollSpeed = backgroundLayer["scrollSpeed"];
        int camDependenceX = backgroundLayer["camDependenceX"];
        int isActuallyScrolling = backgroundLayer["isActuallyScrolling"];
        background.addLayer(filePath,scrollSpeed,camDependenceX,isActuallyScrolling);
    }
}

void core::getAnimationForPlayer(inGameObject* player){
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

int core::getScreenWidth(){return screenWidth;}
int core::getScreenHeight(){return screenHeight;}