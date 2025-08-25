#include <SDL.h>
#include "objectManager.hxx"
#include "time.hxx"
#include "collision.hxx"
#include <iostream>

namespace engine{
    void objectManager::addObject(float x, float y, int width, int height, float MovementSpeed, SDL_Color objectColor, std::string objectTag, int objectInitalRenderCoordinateX, int objectInitalRenderCoordinateY){
        gameObjects.push_back(new inGameObject(x,y,width,height,MovementSpeed,objectColor,objectTag,objectInitalRenderCoordinateX,objectInitalRenderCoordinateY));
    }

    void objectManager::addEnemyObject(float x, float y, int width, int height, float MovementSpeed, SDL_Color objectColor, std::string objectTag, int objectInitalRenderCoordinateX, int objectInitalRenderCoordinateY, float rangeStart, float rangeEnd){
        dynamicObjects.push_back(gameObjects.size());
        gameObjects.push_back(new enemyObject(x,y,width,height,MovementSpeed,objectColor,objectTag,objectInitalRenderCoordinateX,objectInitalRenderCoordinateY,rangeStart,rangeEnd));
    }

    void objectManager::addMovingPlatform(float x, float y, int width, int height, float MovementSpeed, SDL_Color objectColor, std::string objectTag, int objectInitalRenderCoordinateX, int objectInitalRenderCoordinateY, float rangeStart, float rangeEnd){
        dynamicObjects.push_back(gameObjects.size());
        gameObjects.push_back(new movingPlatform(x,y,width,height,MovementSpeed,objectColor,objectTag,objectInitalRenderCoordinateX,objectInitalRenderCoordinateY,rangeStart,rangeEnd));
    }
    
    void objectManager::updateAllObjects(){
        int playerIndex = gameObjects.size()-1;
        gameObjects[playerIndex]->updateObjectState(engineTime::getDeltaTime(),levelWidth,levelHeight);
        gameObjects[playerIndex]->setGrounded(false);

        for(int i : dynamicObjects){
            if(gameObjects[i]->getObjectTag()=="MovingPlatform"||gameObjects[i]->getObjectTag()=="Enemy")
                gameObjects[i]->updateObjectState(engineTime::getDeltaTime(),levelWidth,levelHeight);
        }

        bool grounded=false;

        for(inGameObject* surface : gameObjects){
            if(surface->getObjectTag()=="Wall"){
                if(collision::checkAABB(*gameObjects[playerIndex],*surface)){
                    collision::resolveCollision(*gameObjects[playerIndex],*surface);
                }

                if(collision::isTouchingGround(*gameObjects[playerIndex],*surface)){
                    grounded = true;
                }
            }
            else if (surface->getObjectTag()=="MovingPlatform"){
                if(collision::checkAABB(*gameObjects[playerIndex],*surface)){
                    collision::resolveCollision(*gameObjects[playerIndex],*surface);
                }
                if(collision::isTouchingGround(*gameObjects[playerIndex],*surface)){
                    grounded = true;
                    gameObjects[playerIndex]->setX(gameObjects[playerIndex]->getX()+static_cast<movingPlatform*>(surface)->getDeltaX());
                }
            }
            else if (surface->getObjectTag()=="Enemy"){
                if(collision::checkAABB(*gameObjects[playerIndex],*surface)){
                    collision::resolveCollision(*gameObjects[playerIndex],*surface);
                }
                if(collision::isTouchingGround(*gameObjects[playerIndex],*surface)){
                    grounded = true;
                }
            }
        }
        gameObjects[playerIndex]->setGrounded(grounded);
    }

    void objectManager::renderAllObjects(SDL_Renderer* renderer, camera& cam,texture& textureClass){
        for(inGameObject* i : gameObjects){
            i->renderObject(renderer,cam,textureClass);
        }
    }

    void objectManager::removeObject(std::string removeObjectTag){
        for(std::vector<inGameObject*>::iterator i = gameObjects.begin(); i!=gameObjects.end();i++){
            if((*i)->getObjectTag()==removeObjectTag){
                delete *i;
                gameObjects.erase(i);
                break;
            }
        }
    }

    inGameObject* objectManager::getObjectByTag(const std::string& tag) {
        for (inGameObject* obj : gameObjects) {
            if (obj->getObjectTag() == tag) {
                return obj;
            }
        }
        return nullptr; 
    }

    inGameObject* objectManager::getPlayerObject(){
        return gameObjects.back();
    }

    void objectManager::setLevelWidth(int init_levelWidth){
        levelWidth = init_levelWidth;
    }

    void objectManager::setLevelHeight(int init_levelHeight){
        levelHeight = init_levelHeight;
    }

    int objectManager::getLevelWidth(){
        return levelWidth;
    }

    int objectManager::getLevelHeight(){
        return levelHeight;
    }

    void objectManager::clearAllObjects(){
        for (inGameObject* obj : gameObjects) {
            delete obj;
        }
        dynamicObjects.clear();
        gameObjects.clear();
    }
}