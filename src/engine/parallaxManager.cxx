#include "parallaxManager.hxx"
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <iostream>

parallaxManager::parallaxManager(SDL_Renderer* init_renderer, int init_screenWidth, int init_screenHeight) : renderer(init_renderer), screenWidth(init_screenWidth), screenHeight(init_screenHeight) {}

parallaxManager::~parallaxManager(){
    for(auto& i : layers){
        if(i.parallaxTexture) SDL_DestroyTexture(i.parallaxTexture);
    }
    layers.clear();
}

void parallaxManager::addLayer(const std::string& filePath, float scrollSpeed){
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
    tempLayer.offsetX=0;
    tempLayer.textureWidth=screenWidth;
    tempLayer.textureHeight=screenHeight;
    layers.push_back(tempLayer);
}

void parallaxManager::update(float deltaTime){
    for(auto& i : layers){
        i.offsetX -= i.scrollSpeed*deltaTime;
        if(i.offsetX<i.textureWidth){
            i.offsetX += i.textureWidth;
        }
    }
}

void parallaxManager::render(){
    for(auto& i : layers){
        float x = -i.offsetX;
        while (x < screenWidth) {
                SDL_FRect renderablTextureRect = { x, 0.0f, (float)i.textureWidth, (float)i.textureHeight };
                SDL_RenderCopyF(renderer, i.parallaxTexture, nullptr, &renderablTextureRect);
                x += i.textureWidth;
        }
    }
}