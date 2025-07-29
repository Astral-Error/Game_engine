#include "keyBindManager.hxx"
#include <SDL.h>
#include <SDL_Keycode.h>
#include <SDL_Keyboard.h>
#include <fstream>
#include <iostream>
#include "json.hpp"
using json = nlohmann::json;

void keyBindManager::loadKeyBindConfig(const std::string& keyBindConfigFile){
    std::ifstream file(keyBindConfigFile);
    if(!file.is_open()){
        std::cout<<"Error opening file "<<keyBindConfigFile<<std::endl;
    }
    json data;
    file>>data;
    for(auto& [action,key] : data.items()){
        SDL_Scancode scanCode = SDL_GetScancodeFromName(key.get<std::string>().c_str());
        if(scanCode!=SDL_SCANCODE_UNKNOWN){
            actionToKey[action] = scanCode;
        }
        else{
            std::cout<<"Invalid key name for action!"<<std::endl;
        }
    }
}

bool keyBindManager::isActionPressed(const std::string& action){
    const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
    auto it = actionToKey.find(action);
    return it!=actionToKey.end()&&keyboardState[it->second];
}

SDL_Scancode keyBindManager::getKeyForAction(const std::string& action) {
    return actionToKey.count(action) ? actionToKey[action] : SDL_SCANCODE_UNKNOWN;
}