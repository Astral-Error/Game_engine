#include "time.hxx"
#include <SDL.h>

namespace engineTime{
    static Uint64 frameStart=0;
    static float deltaTime=0.0f;
    static Uint64 sysTimerFrequency = SDL_GetPerformanceFrequency();

    void startFrame(){
        frameStart=SDL_GetPerformanceCounter();
    }

    void endFrame(int targetFPS){
        Uint64 frameEnd= SDL_GetPerformanceCounter();
        deltaTime = float(frameEnd-frameStart)/sysTimerFrequency;
        Uint64 frameDelay = 1000/targetFPS;

        float targetFrameTime = 1.0/targetFPS;
        if(deltaTime<targetFrameTime){
            SDL_Delay(Uint32(targetFrameTime-deltaTime)*1000.0);
            deltaTime = targetFrameTime;
        }
    }
    float getDeltaTime(){
        return deltaTime;
    }
}