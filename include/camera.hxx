#pragma once

class camera{
    private:
        float cameraX,cameraY;
        int cameraWidth,cameraHeight;
        float cameraSmoothing;
    
    public:
        camera(int,int);
        void updateCamera(float,float);
        float getX();
        float getY();
        int getCameraWidth();
        int getCameraHeight();
};