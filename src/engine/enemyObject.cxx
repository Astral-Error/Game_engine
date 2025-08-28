#include "inGameObject.hxx"
#include <iostream>
#include <cmath>
enemyObject::enemyObject(float init_x, float init_y,
    int init_width, int init_height,
    float init_speed,
    SDL_Color init_objectColor,
    std::string init_objectTag,
    int init_objectInitalRenderCoordinateX, int init_objectInitalRenderCoordinateY,
    float init_rangeStart, float init_rangeEnd,
    float init_attackRange, float init_damageCooldown, float init_cooldownTimer,
    float init_chaseRange) : 
        inGameObject(init_x, init_y,
                   init_width, init_height,
                   init_speed,
                   init_objectColor,
                   init_objectTag,
                   init_objectInitalRenderCoordinateX,
                   init_objectInitalRenderCoordinateY),
        rangeStart(init_rangeStart), rangeEnd(init_rangeEnd),
        attackRange(init_attackRange), damageCooldown(init_damageCooldown), cooldownTimer(init_cooldownTimer),
        state(ENEMY_PATROL), chaseRange(init_chaseRange)
        {}

enemyObject::enemyObject() : inGameObject(), rangeStart(0), rangeEnd(0) {}

void enemyObject::updateObjectState(float deltaTime, int levelWdth, int levelHeight, inGameObject* player){
    cooldownTimer-=deltaTime;
    if(player){
        float distanceFromEnemy = (player->getX()-x<0)? x-(player->getX()+player->getWidth()) : x+width-player->getX();
        if(distanceFromEnemy<=attackRange && cooldownTimer<=0.0){
            state = ENEMY_ATTACK;
        }
        else if(player->getX()>x && x+width+chaseRange>=player->getX()){
            state = ENEMY_CHASE;
        }
        else if (player->getX()+player->getWidth()<x && x-chaseRange<=player->getX()+player->getWidth()){
            state=ENEMY_CHASE;
        }
        else if(!(x>=rangeStart && x<=rangeEnd)){
            state=ENEMY_RETURN;
        }
        else{
            state=ENEMY_PATROL;
        }
    }
    switch(state){
        case ENEMY_PATROL:{
            float velocityX = patrolDirectionRight? movementSpeed:-movementSpeed;
            setX(getX()+velocityX*deltaTime);
            if(getX()<rangeStart) patrolDirectionRight=true;
            else if (getX()+getWidth()>rangeEnd) patrolDirectionRight=false;
            break;
        }

        case ENEMY_CHASE:{
            if(player->getX()+player->getWidth()<=x)
                setX(getX()-movementSpeed*2*deltaTime);
            else
                setX(getX()+movementSpeed*2*deltaTime);
            break;
        }

        case ENEMY_ATTACK:{
            player->damage(1);
            std::cout<<1<<std::endl;
            cooldownTimer = damageCooldown;
            break;
        }

        case ENEMY_RETURN: {
            if(x>rangeStart-1 && x<rangeStart+1){
                state = ENEMY_PATROL;
            }
            else{
                if(x>rangeStart)
                    setX(x-movementSpeed*deltaTime);
                else
                    setX(x+movementSpeed*deltaTime);
            }
            break;
        }
    }
}

void enemyObject::tryAttackPlayer(inGameObject* player){
    float dx = player->getX()+player->getWidth()/2-(x+width/2);
    float dy = player->getY()+player->getHeight()/2-(y+height/2);
    float distSq = dx*dx+dy*dy;
    if(distSq<=attackRange*attackRange && cooldownTimer<=0.0){
        player->damage(1);
        std::cout<<1<<std::endl;
        cooldownTimer = damageCooldown;
        state = ENEMY_ATTACK;
    }
    else{
        state = ENEMY_PATROL;
    }
}

enemyObject::~enemyObject(){

}