#pragma once
#include <SDL_Keycode.h>
#include <SDL.h>
#include <string>
#include <unordered_map>

class keyBindManager{
    private:
        std::unordered_map<std::string, SDL_Scancode> actionToKey;
    
    public:
        void loadKeyBindConfig(const std::string&);
        bool isActionPressed(const std::string&);
        SDL_Scancode getKeyForAction(const std::string&);
};