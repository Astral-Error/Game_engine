#pragma once

class camera{
    private:
        float cameraX,cameraY;
        int cameraWidth,cameraHeight;
        float cameraSmoothing=1.0;
    
    public:
        camera(int,int);
        void updateCamera(float,float);
        float getCameraX();
        float getCameraY();
        int getCameraWidth();
        int getCameraHeight();
};