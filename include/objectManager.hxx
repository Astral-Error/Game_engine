#pragma once
#include <SDL.h>
#include "inGameObject.hxx"
#include <vector>
#include <string>

namespace engine{
    class objectManager{
        private:
            std::vector<inGameObject> gameObjects;

        public:
            void addObject(float,float,int,int,float,SDL_Color,std::string objectTag="NULL");
            void updateAllObjects();
            void renderAllObjects(SDL_Renderer*);
            void removeObject(std::string tag);
    };
}