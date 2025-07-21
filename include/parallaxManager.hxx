#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>

class parallaxManager{
    private:
        struct parallaxLayer{
            SDL_Texture* parallaxTexture;
            float scrollSpeed, offsetX;
            int textureWidth, textureHeight, camDependence;
        };
        SDL_Renderer* renderer;
        std::vector<parallaxLayer> layers;
        int screenWidth, screenHeight;
    
    public:
        parallaxManager(SDL_Renderer*,int,int);
        ~parallaxManager();
        void addLayer(const std::string& filePath, float scrollSpeed, int camDependence=0);
        void update(float);
        void render(float);
};