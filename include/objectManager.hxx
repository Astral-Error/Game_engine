#pragma once
#include <SDL.h>
#include "inGameObject.hxx"
#include <vector>
#include <string>

namespace engine{
    class objectManager{
        private:
            int levelWidth, levelHeight;
            std::vector<inGameObject> gameObjects;

        public:
            void addObject(float,float,int,int,float,SDL_Color,std::string objectTag="NULL");
            void updateAllObjects();
            void renderAllObjects(SDL_Renderer*,camera&,texture&);
            void removeObject(std::string tag);
            void setLevelWidth(int);
            void setLevelHeight(int);
            int getLevelWidth();
            int getLevelHeight();
            inGameObject* getObjectByTag(const std::string& tag);
            inGameObject* getPlayerObject();
            void clearAllObjects();
    };
}