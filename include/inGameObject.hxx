#pragma once
#include <SDL.h>
#include <string>

class inGameObject{
    private:
        float x,y;
        int width, height;
        float movementSpeed, velocityY=0.0f, gravity=980.0;
        SDL_Color objectColor;
        bool isGrounded = false;
        std::string objectTag;
        static constexpr int SCREEN_WIDTH = 1280;
        static constexpr int SCREEN_HEIGHT = 720;

    public:
        inGameObject(float,float,int,int,float,SDL_Color,std::string);
        inGameObject();
        float getX();
        float getY();
        float getWidth();
        float getHeight();
        float getVelocityY();
        float getGrounded();
        std::string getObjectTag();
        float getVelocity();
        void setY(float);
        void setX(float);
        void setVelocityY(float);
        void setGrounded(bool);
        void updateObjectState(float);
        void renderObject(SDL_Renderer*);
};