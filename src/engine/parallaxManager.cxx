#include "parallaxManager.hxx"
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <iostream>
#include <cmath>
parallaxManager::parallaxManager():renderer(nullptr),screenWidth(0),screenHeight(0){}
parallaxManager::parallaxManager(SDL_Renderer* init_renderer, int init_screenWidth, int init_screenHeight) : renderer(init_renderer), screenWidth(init_screenWidth), screenHeight(init_screenHeight) {}

parallaxManager::~parallaxManager(){
    for(auto& i : layers){
        if(i.parallaxTexture) SDL_DestroyTexture(i.parallaxTexture);
    }
    layers.clear();
}

void parallaxManager::addLayer(const std::string& filePath, float scrollSpeed, int camDependenceX, int isActuallyScrolling){
    SDL_Surface* tempSurface = IMG_Load(filePath.c_str());
    if(!tempSurface){
        std::cout<<"Error in loading image, Error: "<<IMG_GetError()<<std::endl;
        return;
    }
    SDL_Texture* parallaxTexture = SDL_CreateTextureFromSurface(renderer,tempSurface);
    if(!parallaxTexture){
        std::cout<<"Error creating texture, Error: "<<SDL_GetError()<<std::endl;
        return;
    }
    SDL_FreeSurface(tempSurface);
    parallaxLayer tempLayer;
    tempLayer.parallaxTexture=parallaxTexture;
    tempLayer.scrollSpeed=scrollSpeed;
    if (scrollSpeed < farthestLayerScroll) farthestLayerScroll = scrollSpeed;
    tempLayer.offsetX=0;
    tempLayer.textureWidth=screenWidth;
    tempLayer.textureHeight=screenHeight;
    tempLayer.camDependenceX=camDependenceX;
    tempLayer.isActuallyScrolling=isActuallyScrolling;
    layers.push_back(tempLayer);
}

void parallaxManager::update(float deltaTime){
        for(auto& i : layers){
            if(i.isActuallyScrolling) i.offsetX -= i.scrollSpeed*deltaTime;
            if(i.offsetX<i.textureWidth){
                i.offsetX += i.textureWidth;
            }
        }
}

void parallaxManager::render(float cameraX){
    for(auto& i : layers){
        float x;
        if(i.camDependenceX==0){
            x = -fmod(i.offsetX + cameraX * (farthestLayerScroll), i.textureWidth);
        }
        else{
            x = -fmod(i.offsetX + cameraX * (i.scrollSpeed), i.textureWidth);
        }
        while (x < screenWidth) {
                SDL_FRect renderablTextureRect = { x, 0.0, (float)i.textureWidth, (float)i.textureHeight };
                SDL_RenderCopyF(renderer, i.parallaxTexture, nullptr, &renderablTextureRect);
                x += i.textureWidth;
        }
    }
}