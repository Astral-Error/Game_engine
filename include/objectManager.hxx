#pragma once
#include <SDL.h>
#include "inGameObject.hxx"
#include "texture.hxx"
#include "renderer.hxx"
#include <vector>
#include <string>

namespace engine{
    class objectManager{
        private:
            int levelWidth, levelHeight;
            std::vector<inGameObject*> gameObjects;
            std::vector<int> dynamicObjects;

        public:
            void addObject(float,float,int,int,float,SDL_Color,std::string objectTag="NULL", int objectInitalRenderCoordinateX=0, int objectInitalRenderCoordinateY=0);
            void addMovingPlatform(float, float, int, int, float, SDL_Color, std::string, int, int, float, float);
            void addEnemyObject(float, float, int, int, float, SDL_Color, std::string, int, int, float, float, float init_attackRange=40, float init_damageCooldown = 0.5, float init_cooldownTimer=0.0, float init_chaseRange=200);
            void updateAllObjects(texture&);
            void renderAllObjects(Renderer*,camera&,texture&);
            void removeObject(std::string tag);
            void setLevelWidth(int);
            void setLevelHeight(int);
            int getLevelWidth();
            int getLevelHeight();
            inGameObject* getObjectByTag(const std::string& tag);
            inGameObject* getPlayerObject();
            void clearAllObjects();
            void checkEnemyAttacks(float);
    };
}