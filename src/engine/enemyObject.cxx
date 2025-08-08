#include "inGameObject.hxx"

enemyObject::enemyObject(float init_x, float init_y,
    int init_width, int init_height,
    float init_speed,
    SDL_Color init_objectColor,
    std::string init_objectTag,
    int init_objectInitalRenderCoordinateX, int init_objectInitalRenderCoordinateY,
    float init_rangeStart, float init_rangeEnd) : 
        inGameObject(init_x, init_y,
                   init_width, init_height,
                   init_speed,
                   init_objectColor,
                   init_objectTag,
                   init_objectInitalRenderCoordinateX,
                   init_objectInitalRenderCoordinateY),
        rangeStart(init_rangeStart), rangeEnd(init_rangeEnd)
        {}

enemyObject::enemyObject() : inGameObject(), rangeStart(0), rangeEnd(0) {}

void enemyObject::updateObjectState(float deltaTime, int levelWdth, int levelHeight){
    float velocityX = patrolDirectionRight? movementSpeed:-movementSpeed;
    setX(getX()+velocityX*deltaTime);
    if(getX()<rangeStart) patrolDirectionRight=true;
    else if (getX()+getWidth()>rangeEnd) patrolDirectionRight=false;
}