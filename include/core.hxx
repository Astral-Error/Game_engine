#pragma once
#include "window.hxx"

class core{
    private:
        window win;
    
    public:
        bool initiateWindow(const char*,int,int);
        void initiateGameLoop();
};