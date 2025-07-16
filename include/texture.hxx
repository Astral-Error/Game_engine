#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>

class texture{
    private:
        SDL_Texture* texture;
        int textureHeight, textureWidth;
    
    public:
        texture();
        ~texture();
        SDL_Texture* getTexture();
        int getTextureHeight();
        int getTextureWidth();
        bool loadTextureFromFile(SDL_Renderer*,std::string&);
};