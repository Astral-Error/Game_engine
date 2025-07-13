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
                    float playerLeft   = gameObjects[playerIndex].getX();
                    float playerRight  = playerLeft + gameObjects[playerIndex].getWidth();
                    float playerTop    = gameObjects[playerIndex].getY();
                    float playerBottom = playerTop + gameObjects[playerIndex].getHeight();

                    float platLeft   = surface.getX();
                    float platRight  = platLeft + surface.getWidth();
                    float platTop    = surface.getY();
                    float platBottom = platTop + surface.getHeight();

                    if (playerBottom <= platTop + 10.0f) {
                        gameObjects[playerIndex].setY(platTop - gameObjects[playerIndex].getHeight());
                        gameObjects[playerIndex].setVelocityY(0);
                        gameObjects[playerIndex].setGrounded(true);
                    }
                    
                    else if (playerTop >= platBottom - 10.0f) {
                        gameObjects[playerIndex].setY(platBottom);
                        gameObjects[playerIndex].setVelocityY(0);
                    }
                    
                    else if (playerRight >= platLeft && playerLeft < platLeft) {
                        gameObjects[playerIndex].setX(platLeft - gameObjects[playerIndex].getWidth());
                    }
                    
                    else if (playerLeft <= platRight && playerRight > platRight) {
                        gameObjects[playerIndex].setX(platRight);
                    }

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