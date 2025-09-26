#include "texture.hxx"
#include "renderer.hxx"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

texture::~texture(){
    destroy();
}

void texture::addTexture(SDL_Renderer* /*win*/, const std::string& filePath, std::string objectType){
    individualTexture temp;
    SDL_Surface *surface = IMG_Load(filePath.c_str());
    if(!surface){
        std::cerr<<"IMG_Load failed for " << filePath << " : " << IMG_GetError() << std::endl;
        return;
    }
    SDL_Surface* converted = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(surface);
    if(!converted){
        std::cerr<<"Error in surface format conversion for " << filePath << " : " << SDL_GetError() << std::endl;
        return;
    }

    GLuint tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Use GL_BGRA for SDL surface RGBA32 on many platforms (especially Windows)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, converted->w, converted->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, converted->pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLenum err = glGetError();
    if(err != GL_NO_ERROR){
        std::cerr << "GL error after tex upload for " << filePath << ": 0x" << std::hex << err << std::dec << std::endl;
    }

    temp.loadedTexture = tex;
    temp.textureWidth = converted->w;
    temp.textureHeight = converted->h;
    std::cout<<objectType<<' '<<temp.textureWidth<<' '<<temp.textureHeight<<std::endl;
    textureUNMap[objectType] = temp;

    /*std::cout << "Loaded texture '" << objectType << "' from " << filePath << " -> texID=" << tex
              << " size=" << temp.textureWidth << "x" << temp.textureHeight
              << " glIsTexture=" << (glIsTexture(tex) ? "true" : "false") << std::endl;*/

    SDL_FreeSurface(converted);
}


SDL_Texture* texture::getTexture(std::string objectType){
    //return textureUNMap[objectType].loadedTexture;
    return nullptr;
}

texture::individualTexture* texture::getIndiviualTexture(std::string objectType){
    auto it = textureUNMap.find(objectType);
    if(it == textureUNMap.end()) return nullptr;
    return &it->second;
}

int texture::getTextureHeight(std::string objectType){return textureUNMap[objectType].textureHeight;}
int texture::getTextureWidth(std::string objectType){return textureUNMap[objectType].textureWidth;}

void texture::destroy(){
    for(auto &p : textureUNMap){
        if(p.second.loadedTexture) glDeleteTextures(1, &p.second.loadedTexture);
    }
    textureUNMap.clear();
}