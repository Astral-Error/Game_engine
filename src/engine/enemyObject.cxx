#include "inGameObject.hxx"
#include <iostream>
enemyObject::enemyObject(float init_x, float init_y,
    int init_width, int init_height,
    float init_speed,
    SDL_Color init_objectColor,
    std::string init_objectTag,
    int init_objectInitalRenderCoordinateX, int init_objectInitalRenderCoordinateY,
    float init_rangeStart, float init_rangeEnd,
    float init_attackRange, float init_damageCooldown, float init_cooldownTimer) : 
        inGameObject(init_x, init_y,
                   init_width, init_height,
                   init_speed,
                   init_objectColor,
                   init_objectTag,
                   init_objectInitalRenderCoordinateX,
                   init_objectInitalRenderCoordinateY),
        rangeStart(init_rangeStart), rangeEnd(init_rangeEnd),
        attackRange(init_attackRange), damageCooldown(init_damageCooldown), cooldownTimer(init_cooldownTimer)
        {}

enemyObject::enemyObject() : inGameObject(), rangeStart(0), rangeEnd(0) {}

void enemyObject::updateObjectState(float deltaTime, int levelWdth, int levelHeight){
    cooldownTimer-=deltaTime;
    if(state==ENEMY_PATROL){
        float velocityX = patrolDirectionRight? movementSpeed:-movementSpeed;
        setX(getX()+velocityX*deltaTime);
        if(getX()<rangeStart) patrolDirectionRight=true;
        else if (getX()+getWidth()>rangeEnd) patrolDirectionRight=false;
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