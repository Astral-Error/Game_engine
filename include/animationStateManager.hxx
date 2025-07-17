#ifndef ANIMATIONSTATEMANAGER_HXX
#define ANIMATIONSTATEMANAGER_HXX
#include "animation.hxx"
#include <string>
#include <unordered_map>

class animationStateManager{
    private:
        std::unordered_map<std::string, animation*> animations;
        std::string currentState="";
    
    public:
        void addAnimation(const std::string&, animation);
        void play(const std::string& animationState, bool resetIfSame=false);
        void update(float deltaTime);
        animation* getCurrentAnimation();
        std::string getCurrentStateName();
};

#endif //!ANIMATIONSTATEMANAGER_HXX