#pragma once
#include "window.hxx"

class core{
    private:
        window win;
    
    public:
        bool init(const char*,int,int);
        void initiateGameLoop();
};