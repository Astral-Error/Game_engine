#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "renderer.hxx"
#include <unordered_map>
#include <glad/glad.h>

class texture{
    private:
        struct individualTexture{
            GLuint loadedTexture;
            int textureHeight, textureWidth;
        };
        std::unordered_map<std::string, individualTexture> textureUNMap;
    
    public:
        ~texture();
        SDL_Texture* getTexture(std::string);
        individualTexture* getIndiviualTexture(std::string);
        int getTextureHeight(std::string);
        int getTextureWidth(std::string);
        void addTexture(SDL_Renderer*,const std::string&, std::string);
        void destroy();
        friend class animation;
};