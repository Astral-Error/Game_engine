#include "time.hxx"
#include <SDL.h>

namespace engineTime{
    static Uint64 frameStart=0;
    static Uint64 frameEnd=0;
    static float deltaTime=0.0f;

    void startFrame(){
        frameStart=SDL_GetTicks64();
    }

    void endFrame(int targetFPS){
        frameEnd=SDL_GetTicks64();
        Uint64 frameDuration = frameEnd-frameStart;
        Uint64 frameDelay = 1000/targetFPS;

        if(frameDuration<frameDelay){
            SDL_Delay(frameDelay-frameDuration);
            frameEnd=SDL_GetTicks64();
        }
        deltaTime = (frameEnd-frameStart)/1000.0f;
    }
    float getDeltaTime(){
        return deltaTime;
    }
}