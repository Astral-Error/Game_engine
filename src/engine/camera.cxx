#include "camera.hxx"
#include "algorithm"
#include <iostream>

camera::camera(int init_CameraWidth, int init_CameraHeight) : cameraX(0), cameraY(0), cameraWidth(init_CameraWidth), cameraHeight(init_CameraHeight){}

void camera::updateCamera(float targetX, float targetY, float levelWidth, float levelHeight) {
    float targetCamX = targetX - cameraWidth / 2.0f;
    float targetCamY = targetY - cameraHeight / 2.0f;
    cameraX += (targetCamX - cameraX) * cameraSmoothing;
    cameraY += (targetCamY - cameraY) * cameraSmoothing;
    float maxCameraX = levelWidth - cameraWidth;
    float maxCameraY = levelHeight - cameraHeight;
    if (cameraX < 0) cameraX = 0;
    if (cameraY < 0) cameraY = 0;
    if (cameraX > maxCameraX) cameraX = maxCameraX;
    if (cameraY > maxCameraY) cameraY = maxCameraY;
    std::cout << "CameraX: " << cameraX << " | CameraY: " << cameraY << std::endl;
}

float camera::getCameraX(){return cameraX;}
float camera::getCameraY(){return cameraY;}
int camera::getCameraWidth(){return cameraWidth;}
int camera::getCameraHeight(){return cameraHeight;}