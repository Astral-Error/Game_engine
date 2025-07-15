#pragma once
#include "window.hxx"
#include "inGameObject.hxx"
#include "objectManager.hxx"
#include <SDL.h>
#include <vector>

class core{
    private:
        window win;
        engine::objectManager objManager;
        SDL_Color mediumGrey = {169,169,169,255};
        SDL_Color white = {255,255,255,255};
        SDL_Color purple = {150,160,255,255};
        static SDL_Surface* surfaceTexture;
        static SDL_Texture* wallTexture;
        static SDL_Texture* backgroundImage;
    
    public:
        bool initiateWindow(const char*,int,int);
        void initiateGameLoop();
        void createSampleMap();
        static SDL_Texture* getTexture(std::string);
};