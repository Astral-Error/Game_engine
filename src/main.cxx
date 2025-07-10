#include <iostream>
#include "window.hxx"

int main(int argc, char* argv[]){
    window win;
    if(!win.init("Shadow Swap",1280,720)) return -1;
    while(win.isRunning()){
        win.inputHandler();
    }
    return 0;
}