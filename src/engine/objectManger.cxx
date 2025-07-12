#include <SDL.h>
#include "objectManager.hxx"
#include "time.hxx"
#include "collision.hxx"

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

        int playerIndex = gameObjects.size()-1;

        for(objectWithTag surface : gameObjects){
            if(surface.objectTag=="Wall"){
                if(collision::checkAABB(gameObjects[playerIndex].object,surface.object)){
                    float playerLeft   = gameObjects[playerIndex].object.getX();
                    float playerRight  = playerLeft + gameObjects[playerIndex].object.getWidth();
                    float playerTop    = gameObjects[playerIndex].object.getY();
                    float playerBottom = playerTop + gameObjects[playerIndex].object.getHeight();

                    float platLeft   = surface.object.getX();
                    float platRight  = platLeft + surface.object.getWidth();
                    float platTop    = surface.object.getY();
                    float platBottom = platTop + surface.object.getHeight();

                    if (playerBottom <= platTop + 10.0f) {
                        gameObjects[playerIndex].object.setY(platTop - gameObjects[playerIndex].object.getHeight());
                        gameObjects[playerIndex].object.setVelocityY(0);
                        gameObjects[playerIndex].object.setGrounded(true);
                    }
                    
                    else if (playerTop >= platBottom - 10.0f) {
                        gameObjects[playerIndex].object.setY(platBottom);
                        gameObjects[playerIndex].object.setVelocityY(0);
                    }
                    
                    else if (playerRight >= platLeft && playerLeft < platLeft) {
                        gameObjects[playerIndex].object.setX(platLeft - gameObjects[playerIndex].object.getWidth());
                    }
                    
                    else if (playerLeft <= platRight && playerRight > platRight) {
                        gameObjects[playerIndex].object.setX(platRight);
                    }
                }
            }
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