#include "texture.hxx"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

texture::~texture(){
    destroy();
}

void texture::addTexture(SDL_Renderer* win,const std::string& filePath, std::string objectType){
    individualTexture temp;
    SDL_Texture* tempTexture = IMG_LoadTexture(win,filePath.c_str());
    temp.loadedTexture = tempTexture;
    SDL_QueryTexture(temp.loadedTexture,nullptr,nullptr,&temp.textureWidth,&temp.textureHeight);
    textureUNMap[objectType] = temp;
}

/*bool texture::loadTextureFromFile(SDL_Renderer* renderer, const std::string& filePath){
    destroy();
    loadedTexture = IMG_LoadTexture(renderer,filePath.c_str());
    if(!loadedTexture){
        std::cout<<"Failed to load texture at "<<filePath<<"Error: "<<SDL_GetError()<<std::endl;
        return false;
    }
    SDL_QueryTexture(loadedTexture,nullptr,nullptr,&textureWidth,&textureHeight);
    return true;
}*/

SDL_Texture* texture::getTexture(std::string objectType){
    return textureUNMap[objectType].loadedTexture;
}

texture::individualTexture* texture::getIndiviualTexture(std::string objectType){
    return &textureUNMap[objectType];
}

int texture::getTextureHeight(std::string objectType){return textureUNMap[objectType].textureHeight;}
int texture::getTextureWidth(std::string objectType){return textureUNMap[objectType].textureWidth;}

void texture::destroy(){
    /*if(loadedTexture){
        SDL_DestroyTexture(loadedTexture);
        loadedTexture=nullptr;
        textureWidth=0;
        textureHeight=0;
    }*/
}