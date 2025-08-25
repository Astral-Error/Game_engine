#include "inGameObject.hxx"

movingPlatform::movingPlatform(float init_x, float init_y,
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

movingPlatform::movingPlatform() : inGameObject(), rangeStart(0), rangeEnd(0) {}

void movingPlatform::updateObjectState(float deltaTime, int levelWdth, int levelHeight){
    float velocityX = movementDirection? movementSpeed:-movementSpeed;
    setVelocityX(velocityX); 
    deltaX=velocityX*deltaTime;
    setX(getX()+velocityX*deltaTime);
    if(getX()<rangeStart) movementDirection=true;
    else if (getX()+getWidth()>rangeEnd) movementDirection=false;
}

movingPlatform::~movingPlatform(){

}

float movingPlatform::getDeltaX(){return deltaX;}