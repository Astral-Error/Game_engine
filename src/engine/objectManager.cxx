#include <SDL.h>
#include "objectManager.hxx"
#include "time.hxx"
#include "collision.hxx"

namespace engine{
    void objectManager::addObject(float x, float y, int width, int height, float MovementSpeed, SDL_Color objectColor, std::string objectTag){
        inGameObject temp(x,y,width,height,MovementSpeed,objectColor,objectTag);
        gameObjects.push_back(temp);
    }

    void objectManager::updateAllObjects(){
        int playerIndex = gameObjects.size()-1;
        gameObjects[playerIndex].updateObjectState(engineTime::getDeltaTime(),levelWidth,levelHeight);
        gameObjects[playerIndex].setGrounded(false);

        bool grounded=false;

        for(inGameObject surface : gameObjects){
            if(surface.getObjectTag()=="Wall"){
                if(collision::checkAABB(gameObjects[playerIndex],surface)){
                    collision::resolveCollision(gameObjects[playerIndex],surface);
                }

                if(collision::isTouchingGround(gameObjects[playerIndex], surface)){
                    grounded = true;
                }
            }
        }
        gameObjects[playerIndex].setGrounded(grounded);
    }

    void objectManager::renderAllObjects(SDL_Renderer* renderer, camera& cam,texture& textureClass){
        for(inGameObject& i : gameObjects){
            i.renderObject(renderer,cam,textureClass);
        }
    }

    void objectManager::removeObject(std::string removeObjectTag){
        for(std::vector<inGameObject>::iterator i = gameObjects.begin(); i!=gameObjects.end();i++){
            if((*i).getObjectTag()==removeObjectTag){
                gameObjects.erase(i);
                break;
            }
        }
    }

    inGameObject* objectManager::getObjectByTag(const std::string& tag) {
        for (auto& obj : gameObjects) {
            if (obj.getObjectTag() == tag) {
                return &obj;
            }
        }
        return nullptr; 
    }

    inGameObject* objectManager::getPlayerObject(){
        return &gameObjects[gameObjects.size()-1];
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
        gameObjects.clear();
    }
}