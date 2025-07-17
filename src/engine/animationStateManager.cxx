#include "animationStateManager.hxx"
#include <SDL.h>

void animationStateManager::addAnimation(const std::string& animationState, animation sourceAnimation){
    animations[animationState] = &sourceAnimation;
    if(currentState.empty()){
        currentState=animationState;
    }
}

void animationStateManager::play(const std::string& animationState, bool resetIfSame){
    if(currentState==animationState && !resetIfSame) return;
    currentState = animationState;
    animations[currentState]->resetAnimation();
}

void animationStateManager::update(float deltaTime){
    if(animations.find(currentState)!=animations.end()){
        animations[currentState]->updateAnimation(deltaTime);
    }
}

animation* animationStateManager::getCurrentAnimation(){
    if(animations.find(currentState)!=animations.end()){
        return animations[currentState];
    }
    return nullptr;
}

std::string animationStateManager::getCurrentStateName(){
    return currentState;
}