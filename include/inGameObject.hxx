#pragma once
#include <SDL.h>

class inGameObject{
    private:
        float x,y;
        int width, height;
        float movementSpeed;
        SDL_Color objectColor;

    public:
        inGameObject(float,float,int,int,float,SDL_Color);
        inGameObject();
        void updateObjectState(float);
        void renderObject(SDL_Renderer*);
};