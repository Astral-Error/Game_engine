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
        int health=3;
        bool isPlayer=false;
        float invulnTimer=0;


    public:
        animationStateManager animationStaterManagerClass;
        inGameObject(float,float,int,int,float,SDL_Color,std::string,int,int);
        inGameObject();
        float getX();
        float getY();
        float getWidth();
        float getHeight();
        float getVelocityX();
        float getVelocityY();
        float getGrounded();
        std::string getObjectTag();
        int getObjectInitalRenderCoordinateX();
        int getObjectInitalRenderCoordinateY();
        void setY(float);
        void setX(float);
        void setVelocityX(float);
        void addVelocityX(float);
        void setVelocityY(float);
        void setGrounded(bool);
        void setKeyBinds(keyBindManager&);
        void updateJumpBuffer(float);
        virtual void updateObjectState(float,int,int);
        virtual void renderObject(SDL_Renderer*,camera&,texture&);
        virtual ~inGameObject();
        void setAsPlayer(bool);
        bool getIsPlayer();
        void damage(int);
        void updateInvuln(float);
};

enum EnemyState { ENEMY_PATROL, ENEMY_ATTACK, ENEMY_CHASE, ENEMY_RETURN };

class enemyObject: public inGameObject{
    private:
        float rangeStart, rangeEnd, attackRange, damageCooldown, cooldownTimer, chaseRange;
        bool patrolDirectionRight = true;
        EnemyState state; 

    public:
        enemyObject(float, float, int, int, float, SDL_Color, std::string, int, int, float, float, float, float, float, float);
        enemyObject();
        ~enemyObject();
        void updateObjectState(float, int, int,inGameObject* player=nullptr);
        void tryAttackPlayer(inGameObject*);
};

class movingPlatform: public inGameObject{
    private:
        float deltaX=0.0;
        float rangeStart, rangeEnd;
        bool movementDirection = true;

    public:
        movingPlatform(float, float, int, int, float, SDL_Color, std::string, int, int, float, float);
        movingPlatform();
        ~movingPlatform();
        void updateObjectState(float, int, int);
        float getDeltaX();
};