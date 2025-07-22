#pragma once
#include "texture.hxx"
#include <SDL.h>
#include <SDL_image.h>

class animation{
    private:
        texture::individualTexture* spriteSheet;
        int totalFrames,currentFrame,frameWidth,frameHeight;
        float frameTime,elapsedTime;
        bool looping,flipped;

    public:
        animation();
        animation(texture::individualTexture* ,int,float,int);
        ~animation();
        void updateAnimation(float);
        void resetAnimation();
        void setLooping(bool);
        void setFlipping(bool);
        SDL_Rect getCurrentFrameRect();
        texture::individualTexture* getTexture();
        int getFrameWidth();
        SDL_RendererFlip getFlip();
};