#pragma once
#include "animationStateManager.hxx"
#include "camera.hxx"
#include "keyBindManager.hxx"
#include <SDL.h>
#include <string>

class inGameObject{
    protected:
        float x,y;
        int width, height, objectInitalRenderCoordinateX, objectInitalRenderCoordinateY;
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
        keyBindManager* keyBinds;


    public:
        animationStateManager animationStaterManagerClass;
        inGameObject(float,float,int,int,float,SDL_Color,std::string,int,int);
        inGameObject();
        float getX();
        float getY();
        float getWidth();
        float getHeight();
        float getVelocityY();
        float getGrounded();
        std::string getObjectTag();
        int getObjectInitalRenderCoordinateX();
        int getObjectInitalRenderCoordinateY();
        void setY(float);
        void setX(float);
        void setVelocityY(float);
        void setGrounded(bool);
        void setKeyBinds(keyBindManager&);
        void updateJumpBuffer(float);
        void updateObjectState(float,int,int);
        void renderObject(SDL_Renderer*,camera&,texture&);
};

class enemyObject: public inGameObject{
    private:
        float rangeStart, rangeEnd;
        bool patrolDirectionRight = true;

    public:
        enemyObject(float, float, int, int, float, SDL_Color, std::string, int, int, float, float);
        enemyObject();
        void updateObjectState(float, int, int);
};