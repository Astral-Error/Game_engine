#include "window.hxx"
#include <SDL2/SDL_main.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>
#include <iostream>

window::~window(){
    if(renderer) SDL_DestroyRenderer(renderer);
    if(win) SDL_DestroyWindow(win);
    SDL_Quit();
}

bool window::init(const char* title, int width, int height){
    if(SDL_Init(SDL_INIT_VIDEO)<0){
        std::cout<<"SDL Init failed\n";
        return false;
    }
    win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if(NULL==win){
        std::cout<<"Window creation failed"<<SDL_GetError()<<std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(win,-1,SDL_RENDERER_PRESENTVSYNC);
    if(NULL==renderer){
        std::cout<<"Render Window Creation failed, Error:"<<SDL_GetError()<<std::endl;
    }
    appRunning = true;
    return true;
}

void window::inputHandler(){
    SDL_Event input_event;
    while(SDL_PollEvent(&input_event)){
        switch(input_event.type){
            case SDL_KEYDOWN:
                std::cout<<SDL_GetKeyName(input_event.key.keysym.sym)<<std::endl;
                break;
        }
        if(input_event.type == SDL_QUIT){
            appRunning = false;
        }
    }
}

bool window::isRunning() const{
    return appRunning;
}

SDL_Renderer* window::getRenderer(){
    return window::renderer;
}