#pragma once
#include <SDL.h>
#include <SDL_image.h>

class animation{
    private:
        SDL_Texture* spriteSheet;
        int totalFrames,currentFrame,frameWidth,frameHeight;
        float frameTime,elapsedTime;
        bool looping,flipped;

    public:
        animation();
        animation(SDL_Texture*,int,float,int);
        ~animation();
        void updateAnimation(float);
        void resetAnimation();
        void setLooping(bool);
        void setFlipping(bool);
        SDL_Rect getCurrentFrameRect();
        SDL_Texture* getTexture();
        SDL_RendererFlip getFlip();
};