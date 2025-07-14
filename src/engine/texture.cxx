#include "texture.hxx"
#include <SDL.h>
#include <iostream>

bool texture::loadFromFile(SDL_Renderer* renderer, const std::string& filePath){
    free();
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if(!surface){
        std::cout<<"Failed to load image, Error: "<<IMG_GetError()<<std::endl;
        return false;
    }
    texturePointer = SDL_CreateTextureFromSurface(renderer,surface);
    if(!texturePointer){
        std::cout<<"Failed to load texture, Error: "<<SDL_GetError()<<std::endl;
    }
    else{
        width=surface->w;
        height=surface->h;
    }
    SDL_FreeSurface(surface);
    return texturePointer!=nullptr;
}

void texture::render(SDL_Renderer* renderer, int x, int y, int w, int h){
    if(!texturePointer) return;
    SDL_Rect renderObject = {x,y,w,h};
    SDL_RenderCopy(renderer,texturePointer,nullptr,&renderObject);
}

void texture::free(){
    if(texturePointer){
        SDL_DestroyTexture(texturePointer);
        texturePointer = nullptr;
        width=0;
        height=0;
    }
}