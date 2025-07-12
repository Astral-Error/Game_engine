#pragma once
#include <SDL.h>

class inGameObject{
    private:
        float x,y;
        int width, height;
        float movementSpeed, velocityY=0.0f, gravity=980.0;
        SDL_Color objectColor;
        bool isGrounded = false;

    public:
        inGameObject(float,float,int,int,float,SDL_Color);
        inGameObject();
        float getX();
        float getY();
        float getWidth();
        float getHeight();
        float getVelocityY();
        float getGrounded();
        void setY(float);
        void setX(float);
        void setVelocityY(float);
        void setGrounded(bool);
        void updateObjectState(float);
        void renderObject(SDL_Renderer*);
};