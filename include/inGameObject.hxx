#pragma once
#include "animationStateManager.hxx"
#include "camera.hxx"
#include <SDL.h>
#include <string>

class inGameObject{
    private:
        float x,y;
        int width, height;
        float movementSpeed,
            velocityX=0.0,
            velocityY=0.0f, 
            gravity=980.0,
            groundedGraceTimer = 0.0,
            jumpBufferTimer = 0.0;
        static constexpr float jumpBufferGap=0.1;
        SDL_Color objectColor;
        bool isGrounded = false;
        std::string objectTag;


    public:
        animationStateManager animationStaterManagerClass;
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
        void updateJumpBuffer(float);
        void updateObjectState(float,int,int);
        void renderObject(SDL_Renderer*,camera&,texture&);
};