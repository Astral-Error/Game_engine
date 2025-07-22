#include "inGameObject.hxx"
#include "core.hxx"
#include "objectManager.hxx"
#include "camera.hxx"
#include "main.hxx"
#include <SDL.h>
#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <iostream>

inGameObject::inGameObject(float init_x, float init_y, int init_width, int init_height, float init_speed, SDL_Color init_objectColor, std::string init_objectTag) : x(init_x), y(init_y), width(init_width), height(init_height), movementSpeed(init_speed), objectColor(init_objectColor), objectTag(init_objectTag) {}
inGameObject::inGameObject() : x(0.0), y(0.0), width(0), height(0), movementSpeed(0) {}

void inGameObject::updateObjectState(float deltaTime,int levelWidth,int levelHeight) {
    const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
    velocityX = 0.0;
    if (keyboardState[SDL_SCANCODE_A]){
        velocityX = -2.5f;
        if (animationStaterManagerClass.getCurrentAnimation()) {
            animationStaterManagerClass.getCurrentAnimation()->setFlipping(true); 
        }
    }
    if (keyboardState[SDL_SCANCODE_D]) {
        velocityX = 2.5f;
        if (animationStaterManagerClass.getCurrentAnimation()) {
            animationStaterManagerClass.getCurrentAnimation()->setFlipping(false); 
        }
    }
    x+=velocityX*movementSpeed*deltaTime;
    if (objectTag == "Player"){
        if (!isGrounded){
            velocityY += gravity * deltaTime;
        }
        y += velocityY * deltaTime;
        if(keyboardState[SDL_SCANCODE_SPACE]) jumpBufferTimer=jumpBufferGap;
        updateJumpBuffer(deltaTime);
        if ((isGrounded) && jumpBufferTimer>0) {
            velocityY = -800.0;
            isGrounded = false;
            jumpBufferTimer=0;
        }
        if(!keyboardState[SDL_SCANCODE_SPACE]&&velocityY<0.0) velocityY*=0.5;
        if (!isGrounded) {
            if (animationStaterManagerClass.getCurrentStateName() != "jump") {
                animationStaterManagerClass.play("jump");
            }
        }
        else if (velocityX != 0) {
            if (animationStaterManagerClass.getCurrentStateName() != "walk") {
                animationStaterManagerClass.play("walk");
            }
        }
        else {
            if (animationStaterManagerClass.getCurrentStateName() != "idle") {
                animationStaterManagerClass.play("idle");
            }
        }
        animationStaterManagerClass.update(deltaTime);
        if (x < 0) x = 0;
        else if (x + width > levelWidth) x = levelWidth - width;
    }
}

void inGameObject::renderObject(SDL_Renderer* renderer, camera& cam,texture& textureClass) {
    SDL_Rect destRect = { int(x-cam.getCameraX()), int(y-cam.getCameraY()), width, height };

    if (animationStaterManagerClass.getCurrentAnimation()) {
        SDL_Texture* currentTexture = animationStaterManagerClass.getCurrentAnimation()->getTexture()->loadedTexture;
        SDL_Rect srcRect = animationStaterManagerClass.getCurrentAnimation()->getCurrentFrameRect();
        SDL_RendererFlip flip = animationStaterManagerClass.getCurrentAnimation()->getFlip();
        float renderScale = 1.0;
        float xOffset = (width * renderScale - srcRect.w * renderScale) / 2.0f;
        float yOffset = srcRect.h * renderScale - height;
        if(objectTag=="Player")destRect = { int(x - cam.getCameraX() + xOffset), int(y - cam.getCameraY() - yOffset), int(srcRect.w * renderScale), int(srcRect.h * renderScale)};
        SDL_RenderCopyEx(renderer, currentTexture, &srcRect, &destRect, 0, nullptr, flip);
    }
    else if (textureClass.getTexture(objectTag)) {
        int tileW, tileH;
        SDL_QueryTexture(textureClass.getTexture(objectTag),nullptr,nullptr,&tileW,&tileH);
        for(int offsetY =0;offsetY<height;offsetY+=tileH){
            for(int offsetX=0;offsetX<width;offsetX+=tileW){
                SDL_Rect srcRect = {0,0,tileW,tileH};
                SDL_Rect destRect = {int(x + offsetX - cam.getCameraX()), int(y + offsetY - cam.getCameraY()),std::min(tileW,width-offsetX), std::min(tileH,height-offsetY)};
                SDL_RenderCopy(renderer,textureClass.getTexture(objectTag),&srcRect,&destRect);
            }
        }
    }
    else {
        SDL_SetRenderDrawColor(renderer, objectColor.r, objectColor.g, objectColor.b, objectColor.a);
        SDL_RenderFillRect(renderer, &destRect);
    }
}

float inGameObject::getX() { return x; }
float inGameObject::getY() { return y; }
std::string inGameObject::getObjectTag() { return objectTag; }
float inGameObject::getWidth() { return width; }
float inGameObject::getHeight() { return height; }
float inGameObject::getVelocityY() { return velocityY; }
float inGameObject::getGrounded() { return isGrounded; }
float inGameObject::getVelocity() { return velocityY; }
void inGameObject::setX(float newX) { x = newX; }
void inGameObject::setY(float newY) { y = newY; }
void inGameObject::setVelocityY(float newVelocityY) {velocityY = newVelocityY;}
void inGameObject::setGrounded(bool newState) { isGrounded = newState; }
void inGameObject::updateJumpBuffer(float deltaTime){
    jumpBufferTimer-=deltaTime;
}