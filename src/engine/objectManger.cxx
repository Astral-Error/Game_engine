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
}