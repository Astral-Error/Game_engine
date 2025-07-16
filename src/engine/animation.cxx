#include "animation.hxx"
#include <SDL.h>

animation::animation(): spriteSheet(0), totalFrames(0), currentFrame(0), frameWidth(0), frameHeight(0), frameTime(0), elapsedTime(0), looping(false), flipped(false) {}
animation::animation(SDL_Texture* texture, int init_totalFrames, float duration, int spriteHeight) : spriteSheet(texture), totalFrames(init_totalFrames), frameTime(duration), frameHeight(spriteHeight){
    int texWidthTemp;
    SDL_QueryTexture(texture,nullptr,nullptr,&texWidthTemp,nullptr);
    frameWidth = texWidthTemp/init_totalFrames;
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

SDL_Texture* animation::getTexture(){
    return spriteSheet;
}

SDL_RendererFlip animation::getFlip(){
    return flipped?SDL_FLIP_HORIZONTAL:SDL_FLIP_NONE;
}