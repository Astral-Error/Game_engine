#pragma once
#include <SDL.h>
#include <vector>
#include "inGameObject.hxx"
#include "camera.hxx"

struct LightSource{
    float x,y; //World coordinates for the shadow
    float radius; //Effective radius of the shadow
    float intensity; //Range: 0.0-1.0
};

class shadowManager{
    private:
        float globalDirX, globalDirY;
    public:
        shadowManager();
        void setGlobalLight(float init_globalDirX = -1.0, float init_globalDirY=1.0);
        void renderGlobalShadow(const std::vector<inGameObject*>&, camera&);
        void renderPointLightShadows(const std::vector<inGameObject*>&, std::vector<LightSource>&,camera&);
};