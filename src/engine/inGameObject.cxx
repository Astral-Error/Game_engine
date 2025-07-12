#include "inGameObject.hxx"
#include<SDL.h>
#include<SDL_Keyboard.h>
#include<SDL_Keycode.h>

inGameObject::inGameObject(float init_x, float init_y, int init_width, int init_height, float init_speed) : x(init_x), y(init_y), width(init_width), height(init_height), movementSpeed(init_speed) {}
inGameObject::inGameObject() : x(0.0), y(0.0), width(0), height(0), movementSpeed(0) {}

void inGameObject::updateObjectState(float deltaTime){
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
    if(keyboardState[SDL_SCANCODE_W]) y-=movementSpeed*deltaTime;
    if(keyboardState[SDL_SCANCODE_S]) y+=movementSpeed*deltaTime;
    if(keyboardState[SDL_SCANCODE_A]) x-=movementSpeed*deltaTime;
    if(keyboardState[SDL_SCANCODE_D]) x+=movementSpeed*deltaTime;
}

void inGameObject::renderObject(SDL_Renderer* renderer){
    SDL_Rect rectangle = {int(x),int(y),width,height};
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderFillRect(renderer,&rectangle);
}