#include "inGameObject.hxx"
#include<SDL.h>
#include<SDL_Keyboard.h>
#include<SDL_Keycode.h>


inGameObject::inGameObject(float init_x, float init_y, int init_width, int init_height, float init_speed, SDL_Color init_objectColor, std::string init_objectTag) : x(init_x), y(init_y), width(init_width), height(init_height), movementSpeed(init_speed), objectColor(init_objectColor), objectTag(init_objectTag) {}
inGameObject::inGameObject() : x(0.0), y(0.0), width(0), height(0), movementSpeed(0) {}

void inGameObject::updateObjectState(float deltaTime){
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
    if(keyboardState[SDL_SCANCODE_A]) x-=movementSpeed*deltaTime;
    if(keyboardState[SDL_SCANCODE_D]) x+=movementSpeed*deltaTime;
    if(objectTag=="Player"){
        if(!isGrounded) velocityY += gravity*deltaTime;
        y+=velocityY*deltaTime;
        if(keyboardState[SDL_SCANCODE_SPACE]&&isGrounded){
            velocityY = -600.0;
            isGrounded = false;
        }
    }
}

void inGameObject::renderObject(SDL_Renderer* renderer){
    SDL_Rect rectangle = {int(x),int(y),width,height};
    SDL_SetRenderDrawColor(renderer,objectColor.r,objectColor.g,objectColor.b,objectColor.a);
    SDL_RenderFillRect(renderer,&rectangle);
}

float inGameObject::getX() {return x;}
float inGameObject::getY() {return y;}
std::string inGameObject::getObjectTag(){return objectTag;}
void inGameObject::setX(float newX){x=newX;}
float inGameObject::getWidth() {return width;}
float inGameObject::getHeight() {return height;}
float inGameObject::getVelocityY() {return velocityY;}
float inGameObject::getGrounded() {return isGrounded;}
void inGameObject::setY(float newY) {y=newY;}
void inGameObject::setVelocityY(float newVelocityY) {velocityY=newVelocityY;}
void inGameObject::setGrounded(bool newState) {isGrounded = newState;}