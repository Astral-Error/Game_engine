#pragma once
#include <SDL.h>
#include "inGameObject.hxx"
#include <vector>
#include <string>

namespace engine{
    class objectManager{
        private:
            struct objectWithTag{
                inGameObject object;
                std::string objectTag;
            };

            std::vector<objectWithTag> gameObjects;

        public:
            void addObject(float,float,int, float,std::string objectTag="NULL");
            void updateAllObjects();
            void renderAllObjects(SDL_Renderer*);
            void removeObject(std::string tag);
    };
}