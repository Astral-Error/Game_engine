#pragma once
#include <SDL2/SDL.h>

class inGameObject{
    private:
        int x,y;
        int width, height;
        int speed;

    public:
        inGameObject(int,int,int);
        void updateObjectState();
        void renderObject();
};