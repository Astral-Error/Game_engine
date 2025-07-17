#pragma once
#include "texture.hxx"
#include <SDL.h>
#include <SDL_image.h>

class animation{
    private:
        texture* spriteSheet;
        int totalFrames,currentFrame,frameWidth,frameHeight;
        float frameTime,elapsedTime;
        bool looping,flipped;

    public:
        animation();
        animation(texture*,int,float,int);
        ~animation();
        void updateAnimation(float);
        void resetAnimation();
        void setLooping(bool);
        void setFlipping(bool);
        SDL_Rect getCurrentFrameRect();
        texture* getTexture();
        SDL_RendererFlip getFlip();
};