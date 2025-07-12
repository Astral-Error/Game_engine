#pragma once
#include <SDL.h>

class inGameObject{
    private:
        float x,y;
        int width, height;
        float movementSpeed;

    public:
        inGameObject(float,float,int,float);
        inGameObject();
        void updateObjectState(float);
        void renderObject(SDL_Renderer*);
};