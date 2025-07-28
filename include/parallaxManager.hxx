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
            int textureWidth, textureHeight, camDependenceX, isActuallyScrolling;
        };
        SDL_Renderer* renderer;
        std::vector<parallaxLayer> layers;
        int screenWidth, screenHeight;
        float farthestLayerScroll=1.0;
    
    public:
        parallaxManager();
        parallaxManager(SDL_Renderer*,int,int);
        ~parallaxManager();
        void addLayer(const std::string& filePath, float scrollSpeed, int camDependenceX=0, int isActuallyScrolling=0);
        void update(float);
        void render(float);
};