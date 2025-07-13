#include <SDL.h>
#include "objectManager.hxx"
#include "time.hxx"
#include "collision.hxx"

namespace engine{
    void objectManager::addObject(float x, float y, int height, int width, float MovementSpeed, SDL_Color objectColor, std::string objectTag){
        inGameObject temp(x,y,height,width,MovementSpeed,objectColor,objectTag);
        gameObjects.push_back(temp);
    }

    void objectManager::updateAllObjects(){
        for(inGameObject& i: gameObjects){
            i.updateObjectState(engineTime::getDeltaTime());
        }

        int playerIndex = gameObjects.size()-1;
        gameObjects[playerIndex].setGrounded(false);

        for(inGameObject surface : gameObjects){
            if(surface.getObjectTag()=="Wall"){
                if(collision::checkAABB(gameObjects[playerIndex],surface)){
                    collision::resolveCollision(gameObjects[playerIndex],surface);
                }
            }
        }
    }

    void objectManager::renderAllObjects(SDL_Renderer* renderer){
        for(inGameObject& i : gameObjects){
            i.renderObject(renderer);
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
}