#include "camera.hxx"
#include "algorithm"
#include <iostream>

camera::camera(int init_CameraWidth, int init_CameraHeight) : cameraX(0), cameraY(0), cameraWidth(init_CameraWidth), cameraHeight(init_CameraHeight){}

void camera::updateCamera(float targetX, float targetY) {
    float targetCamX = targetX - cameraWidth / 2.0f;
    float targetCamY = targetY - cameraHeight / 2.0f;
    cameraX += (targetCamX - cameraX) * cameraSmoothing;
    cameraY += (targetCamY - cameraY) * cameraSmoothing;
    std::cout << "CameraX: " << cameraX << " | CameraY: " << cameraY << std::endl;
}

float camera::getCameraX(){return cameraX;}
float camera::getCameraY(){return cameraY;}
int camera::getCameraWidth(){return cameraWidth;}
int camera::getCameraHeight(){return cameraHeight;}