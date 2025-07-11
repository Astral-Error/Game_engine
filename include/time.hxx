#pragma once

namespace engineTime{
    void startFrame();
    void endFrame(int targetFPS=60);
    float getDeltaTime();
}