#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>

class texture{
    private:
        SDL_Texture* texturePointer = nullptr;
        int width=0;
        int height=0;

    public:
        bool loadFromFile(SDL_Renderer*, const std::string&);
        void render(SDL_Renderer*,int,int,int,int);
        void free();
        SDL_Texture* getRawTexture();
        int getWidth();
        int getHeight();
        ~texture(){
            free();
        }
};