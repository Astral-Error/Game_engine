#include <SDL.h>
#include "objectManager.hxx"
#include "time.hxx"

namespace engine{
    void objectManager::addObject(float x, float y, int height, int width, float MovementSpeed, SDL_Color objectColor, std::string objectTag){
        struct objectWithTag temp;
        temp.object = inGameObject(x,y,height,width,MovementSpeed,objectColor);
        temp.objectTag=objectTag;
        gameObjects.push_back(temp);
    }

    void objectManager::updateAllObjects(){
        for(objectWithTag& i: gameObjects){
            i.object.updateObjectState(engineTime::getDeltaTime());
        }
    }

    void objectManager::renderAllObjects(SDL_Renderer* renderer){
        for(objectWithTag& i : gameObjects){
            i.object.renderObject(renderer);
        }
    }

    void objectManager::removeObject(std::string removeObjectTag){
        for(std::vector<objectWithTag>::iterator i = gameObjects.begin(); i!=gameObjects.end();i++){
            if(i->objectTag==removeObjectTag){
                gameObjects.erase(i);
                break;
            }
        }
    }
}