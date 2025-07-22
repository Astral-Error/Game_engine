#include "animation.hxx"
#include "texture.hxx"
#include <SDL.h>

animation::animation(): spriteSheet(nullptr), totalFrames(0), currentFrame(0), frameWidth(0), frameHeight(0), frameTime(0), elapsedTime(0), looping(false), flipped(false) {}
animation::animation(texture::individualTexture* init_spriteSheet, int init_totalFrames, float duration, int spriteHeight) : spriteSheet(init_spriteSheet), totalFrames(init_totalFrames), frameTime(duration), frameHeight(spriteHeight){
    if(spriteSheet){
        frameWidth = spriteSheet->textureWidth / totalFrames; 
    }
    else{
        frameWidth = 0;
    }
}
animation::~animation(){
    
}


void animation::updateAnimation(float deltaTime){
    elapsedTime+=deltaTime;
    if (elapsedTime>=frameTime){
        elapsedTime-=frameTime;
        currentFrame++;
        if(currentFrame>=totalFrames){
            currentFrame = looping?0:totalFrames-1;
        }
    }
}

void animation::resetAnimation(){
    currentFrame=0;
    elapsedTime=0;
}

void animation::setLooping(bool loopSetter){
    looping=loopSetter;
}

void animation::setFlipping(bool flipSetter){
    flipped = flipSetter;
}

SDL_Rect animation::getCurrentFrameRect(){
    SDL_Rect temp;
    temp.x = currentFrame * frameWidth;
    temp.y = 0;
    temp.w = frameWidth;
    temp.h = frameHeight;
    return temp;
}

texture::individualTexture* animation::getTexture(){
    return spriteSheet;
}

SDL_RendererFlip animation::getFlip(){
    return flipped?SDL_FLIP_HORIZONTAL:SDL_FLIP_NONE;
}

int animation::getFrameWidth(){
    return frameWidth;
}