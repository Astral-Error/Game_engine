#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>

class texture{
    private:
        SDL_Texture* loadedTexture;
        int textureHeight, textureWidth;
    
    public:
        texture();
        texture(SDL_Renderer*,const std::string&);
        ~texture();
        SDL_Texture* getTexture();
        int getTextureHeight();
        int getTextureWidth();
        bool loadTextureFromFile(SDL_Renderer*,const std::string&);
        void destroy();
};