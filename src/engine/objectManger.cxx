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
                    inGameObject& player = gameObjects[playerIndex];
                    float playerLeft   = player.getX();
                    float playerRight  = playerLeft + player.getWidth();
                    float playerTop    = player.getY();
                    float playerBottom = playerTop + player.getHeight();

                    float platLeft   = surface.getX();
                    float platRight  = platLeft + surface.getWidth();
                    float platTop    = surface.getY();
                    float platBottom = platTop + surface.getHeight();

                    float overlapX = std::min(playerRight, platRight) - std::max(playerLeft, platLeft);
                    float overlapY = std::min(playerBottom, platBottom) - std::max(playerTop, platTop);

                    if (overlapX <= 0 || overlapY <= 0) return;

                    if (player.getVelocityY() >= 0 && playerBottom - overlapY <= platTop){
                        player.setY(platTop - player.getHeight());
                        player.setVelocityY(0);
                        player.setGrounded(true);
                    }
                    else if (player.getVelocityY() < 0 && playerTop + overlapY >= platBottom){
                        player.setY(platBottom);
                        player.setVelocityY(0);
                    }
                    else{
                        if(playerRight - overlapX <= platLeft){
                            player.setX(platLeft - player.getWidth());
                        }
                        else if(playerLeft + overlapX >= platRight){
                            player.setX(platRight);
                        }
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