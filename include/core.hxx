#pragma once
#include "window.hxx"
#include "inGameObject.hxx"
#include "objectManager.hxx"
#include <vector>

class core{
    private:
        window win;
        engine::objectManager objManager;
    
    public:
        bool initiateWindow(const char*,int,int);
        void initiateGameLoop();
};