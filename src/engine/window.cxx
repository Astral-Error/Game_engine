#include "window.hxx"
#include <SDL_main.h>
#include <SDL.h>
#include <SDL_render.h>
#include <SDL_events.h>
#include <iostream>

window::~window(){
    if(renderer) SDL_DestroyRenderer(renderer);
    if(glContext) SDL_GL_DeleteContext(glContext);
    if(win) SDL_DestroyWindow(win);
    SDL_Quit();
}

bool window::initiateWindow(const char* winTitle, int width, int height){
    Uint32 windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    win = SDL_CreateWindow(winTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, windowFlags);
    if(NULL==win){
        std::cout<<"Window creation failed"<<SDL_GetError()<<std::endl;
        return false;
    }

    glContext = SDL_GL_CreateContext(win);
    if(NULL==glContext){
        std::cout<<"Failed to create OpenGL context: "<<SDL_GetError()<<std::endl;
        return false;
    }
    SDL_GL_SetSwapInterval(1); 

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); 
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
        if(input_event.type == SDL_QUIT){
            appRunning = false;
        }
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

bool window::isRunning() const{
    return appRunning;
}

SDL_Renderer* window::getRenderer(){
    return window::renderer;
}

SDL_Window* window::getWindow(){
    return win;
}

SDL_GLContext window::getGLContext(){
    return glContext;
}