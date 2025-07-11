#pragma once
#include "window.hxx"
#include "inGameObject.hxx"
#include <vector>

class core{
    private:
        window win;
        std::vector<inGameObject> gameObjects;
    
    public:
        bool initiateWindow(const char*,int,int);
        void initiateGameLoop();
};