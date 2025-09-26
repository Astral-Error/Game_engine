#include "window.hxx"
#include <SDL_main.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <SDL_events.h>
#include <iostream>
#include <glad/glad.h>

window::~window(){
    if(renderer){
        renderer->destroy();
        delete renderer;
        renderer=nullptr;
    }
    if(win) SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
}

bool window::initiateWindow(const char* winTitle, int width, int height){
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)!=0){
        std::cout<<"SDL init failed: "<<SDL_GetError()<<std::endl;
        return false;
    }

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if((IMG_Init(imgFlags)&imgFlags)!=imgFlags){
        std::cout<<"IMG_Init failed: "<<IMG_GetError()<<std::endl;
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    Uint32 windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN;
    win = SDL_CreateWindow(winTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, windowFlags);
    if(NULL==win){
        std::cout<<"Window creation failed"<<SDL_GetError()<<std::endl;
        return false;
    }

    renderer = new Renderer();
    if(!renderer->initOpenGL(win,width,height)){
        std::cout<<"Renderer init failed\n";
        return false;
    }
    SDL_GL_SetSwapInterval(1);
    appRunning = true;
    return true;
}

void window::inputHandler(){
    SDL_Event input_event;
    while(SDL_PollEvent(&input_event)){
        if(input_event.type == SDL_QUIT){
            appRunning = false;
        }
        if(input_event.type == SDL_KEYDOWN){
            switch(input_event.key.keysym.sym){
                case SDLK_ESCAPE:
                    appRunning = false;
                    break;

                case SDLK_w:
                    std::cout<<SDL_GetKeyName(input_event.key.keysym.sym);
                    break;

                case SDLK_s:
                    std::cout<<SDL_GetKeyName(input_event.key.keysym.sym);
                    break;
                
                case SDLK_a:
                    std::cout<<SDL_GetKeyName(input_event.key.keysym.sym);
                    break;

                case SDLK_d:
                    std::cout<<SDL_GetKeyName(input_event.key.keysym.sym);
                    break;
                
                default:
                    break;
            }
        }
    }
}

bool window::isRunning() const{
    return appRunning;
}

Renderer* window::getRenderer(){
    return renderer;
}