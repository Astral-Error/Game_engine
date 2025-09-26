#pragma once
#include <glad/glad.h>
#include <renderer.hxx>
#include <vector>
#include <string>

class parallaxManager{
    private:
        struct parallaxLayer{
            GLuint parallaxTexture;
            float scrollSpeed, offsetX;
            int textureWidth, textureHeight, camDependenceX, isActuallyScrolling;
        };
        Renderer* renderer;
        std::vector<parallaxLayer> layers;
        int screenWidth, screenHeight;
        float farthestLayerScroll=1.0;
    
    public:
        parallaxManager();
        parallaxManager(Renderer*,int,int);
        ~parallaxManager();
        void addLayer(const std::string& filePath, float scrollSpeed, int camDependenceX=0, int isActuallyScrolling=0);
        void update(float);
        void render(float);
};