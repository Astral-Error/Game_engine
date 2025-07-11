#include "inGameObject.hxx"
#include<SDL.h>
#include<SDL_Keyboard.h>
#include<SDL_Keycode.h>

inGameObject::inGameObject(int init_x, int init_y, int size) : x(init_x), y(init_y), width(size), height(size), movementSpeed(5) {}

void inGameObject::updateObjectState(){
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
    if(keyboardState[SDL_SCANCODE_W]) y-=movementSpeed;
    if(keyboardState[SDL_SCANCODE_S]) y+=movementSpeed;
    if(keyboardState[SDL_SCANCODE_A]) x-=movementSpeed;
    if(keyboardState[SDL_SCANCODE_D]) x+=movementSpeed;
}

void inGameObject::renderObject(SDL_Renderer* renderer){
    SDL_Rect rectangle = {x,y,width,height};
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderFillRect(renderer,&rectangle);
}