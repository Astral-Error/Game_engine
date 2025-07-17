#include "texture.hxx"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

texture::texture() : loadedTexture(nullptr), textureHeight(0), textureWidth(0){}
texture::~texture(){
    destroy();
}

bool texture::loadTextureFromFile(SDL_Renderer* renderer, const std::string& filePath){
    destroy();
    loadedTexture = IMG_LoadTexture(renderer,filePath.c_str());
    if(!loadedTexture){
        std::cout<<"Failed to load texture at "<<filePath<<"Error: "<<SDL_GetError()<<std::endl;
        return false;
    }
    SDL_QueryTexture(loadedTexture,nullptr,nullptr,&textureWidth,&textureHeight);
    return true;
}

SDL_Texture* texture::getTexture(){
    return loadedTexture;
}

int texture::getTextureHeight(){return textureHeight;}
int texture::getTextureWidth(){return textureWidth;}

void texture::destroy(){
    if(loadedTexture){
        SDL_DestroyTexture(loadedTexture);
        loadedTexture=nullptr;
        textureWidth=0;
        textureHeight=0;
    }
}