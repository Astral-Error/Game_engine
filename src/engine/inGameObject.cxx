#include "inGameObject.hxx"
#include "core.hxx"
#include "objectManager.hxx"
#include <SDL.h>
#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include <iostream>

inGameObject::inGameObject(float init_x, float init_y, int init_width, int init_height, float init_speed, SDL_Color init_objectColor, std::string init_objectTag) : x(init_x), y(init_y), width(init_width), height(init_height), movementSpeed(init_speed), objectColor(init_objectColor), objectTag(init_objectTag) {}
inGameObject::inGameObject() : x(0.0), y(0.0), width(0), height(0), movementSpeed(0) {}

void inGameObject::updateObjectState(float deltaTime) {
    const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
    velocityX = 0.0;
    if (keyboardState[SDL_SCANCODE_A]){
        velocityX = -1.0f;
        if (animationStaterManagerClass.getCurrentAnimation()) {
            animationStaterManagerClass.getCurrentAnimation()->setFlipping(true); 
        }
    }
    if (keyboardState[SDL_SCANCODE_D]) {
        velocityX = 1.0f;
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
            velocityY = -700.0;
            isGrounded = false;
            jumpBufferTimer=0;
        }
        if(!keyboardState[SDL_SCANCODE_SPACE]&&velocityY<0.0) velocityY*=0.5;
        if (y > SCREEN_HEIGHT) {
            y = -height;
            velocityY = 300.0;
            isGrounded = false;
        }
        if (y < -height) {
            y = -height;
        }
        if (velocityX!=0){
            animationStaterManagerClass.play("walk");
        }
        else{
            animationStaterManagerClass.play("idle");
        }
        animationStaterManagerClass.update(deltaTime);
    }

    if (x < 0) {
        x = 0;
    } else if (x + width > SCREEN_WIDTH) {
        x = SCREEN_WIDTH - width;
    }
}

void inGameObject::renderObject(SDL_Renderer* renderer) {
    SDL_Rect destRect;
    if(objectTag=="Player")destRect = { int(x), int(y), width, height };
    else destRect = { int(x), int(y), width, height };

    if (animationStaterManagerClass.getCurrentAnimation()) {
        // Get texture, source frame, and flip direction
        texture* currentTexture = animationStaterManagerClass.getCurrentAnimation()->getTexture();
        SDL_Rect srcRect = animationStaterManagerClass.getCurrentAnimation()->getCurrentFrameRect();
        SDL_RendererFlip flip = animationStaterManagerClass.getCurrentAnimation()->getFlip();

        // Render current animation frame
        SDL_RenderCopyEx(renderer, currentTexture->getTexture(), &srcRect, &destRect, 0, nullptr, flip);
    }
    else if (core::getTexture(objectTag)) {
        // Render full texture if no animation
        SDL_RenderCopy(renderer, core::getTexture(objectTag), nullptr, &destRect);
    }
    else {
        // Fallback: draw solid color box
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