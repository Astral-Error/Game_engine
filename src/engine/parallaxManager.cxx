#include "parallaxManager.hxx"
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <iostream>
#include <cmath>
parallaxManager::parallaxManager():renderer(nullptr),screenWidth(0),screenHeight(0){}
parallaxManager::parallaxManager(Renderer* init_renderer, int init_screenWidth, int init_screenHeight) : renderer(init_renderer), screenWidth(init_screenWidth), screenHeight(init_screenHeight) {}

parallaxManager::~parallaxManager(){
    for(auto& i : layers){
        if(i.parallaxTexture) glDeleteTextures(1,&i.parallaxTexture);
    }
    layers.clear();
}

void parallaxManager::addLayer(const std::string& filePath, float scrollSpeed, int camDependenceX, int isActuallyScrolling){
    SDL_Surface *surface = IMG_Load(filePath.c_str());
    if(!surface){
        std::cerr<<"Couldn't create surface: "<<SDL_GetError()<<std::endl;
        return;
    }
    surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    if(!surface){
        std::cerr<<"Error in surface format conversion: "<<SDL_GetError()<<std::endl;
        return;
    }

    GLuint tex;
    glGenTextures(1,&tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0,
             GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);

    parallaxLayer tempLayer;
    tempLayer.parallaxTexture=tex;
    tempLayer.scrollSpeed=scrollSpeed;
    if (scrollSpeed < farthestLayerScroll) farthestLayerScroll = scrollSpeed;
    tempLayer.offsetX=0;
    tempLayer.textureWidth=screenWidth; //mark for review
    tempLayer.textureHeight=screenHeight; //mark for review
    tempLayer.camDependenceX=camDependenceX;
    tempLayer.isActuallyScrolling=isActuallyScrolling;
    layers.push_back(tempLayer);
    SDL_FreeSurface(surface);
}

void parallaxManager::update(float deltaTime){
        for(auto& i : layers){
            if(i.isActuallyScrolling) i.offsetX -= i.scrollSpeed*deltaTime;
            if(i.offsetX<i.textureWidth){
                i.offsetX += i.textureWidth;
            }
        }
}

void parallaxManager::render(float cameraX) {
    if (!renderer) return;
    for (auto& i : layers) {
        float x;
        if (i.camDependenceX == 0) {
            x = -fmod(i.offsetX + cameraX * (farthestLayerScroll), i.textureWidth);
        } else {
            x = -fmod(i.offsetX + cameraX * (i.scrollSpeed), i.textureWidth);
        }
        while (x < screenWidth) {
            float u0 = 0.0f, v0 = 0.0f, u1 = 1.0f, v1 = 1.0f;
            renderer->submitQuad(i.parallaxTexture, x, 0.0f,
                                 (float)i.textureWidth, (float)i.textureHeight,
                                 u0, v0, u1, v1, 1.0f);
            x += i.textureWidth;
        }
    }
}

