#include "texture.hxx"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

texture::texture() : texture(nullptr), textureHeight(0), textureWidth(0){}
texture::~texture(){
    destroy();
}

bool texture::loadTextureFromFile(SDL_Renderer* renderer, std::string filePath){
    destroy();
    texture = IMG_LoadTexture(renderer,filePath.c_str());
    if(!texture){
        std::cout<<"Failed to load texture at "<<filepath<<"Error: "<<SDL_GetError()<<std::endl;
        return false;
    }
    SDL_QueryTexture(texture,nullptr,nullptr,&textureWidth,&textureHeight);
    return true;
}

SDL_Texture* texture::getTexture(){
    return texture;
}

int getTextureHeight(){return textureHeight;}
int getTextureWidth(){return textureWidth;}

void texture::destroy(){
    if(texture){
        SDL_DestroyTexture(texture);
        texture=nullptr;
        width=0;
        height=0;
    }
}