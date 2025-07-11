#pragma once
#include <SDL.h>

class inGameObject{
    private:
        int x,y;
        int width, height;
        int movementSpeed;

    public:
        inGameObject(int,int,int);
        void updateObjectState();
        void renderObject(SDL_Renderer*);
};