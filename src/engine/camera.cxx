#include "camera.hxx"

camera::camera(int init_CameraWidth, int init_CameraHeight) : x(0), y(0), cameraWidth(init_CameraWidth), cameraHeight(init_CameraHeight){}

void camera::updateCamera(float newX, float newY){
    x += ((newX-cameraWidth/2)-x)*cameraSmoothing;
    y += ((newY-cameraHeight/2-y))*cameraSmoothing;
    x = std::max(0.0f,x);
    y = std::max(0.0f,y);
}

float camera::getCameraX(){return cameraX;}
float camera::getCameraY(){return cameraY;}
int camera::getCameraWidth(){return cameraWidth;}
int camera::getCameraHeight(){return cameraHeight;}