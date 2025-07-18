#pragma once

namespace engineTime{
    void startFrame();
    void endFrame(int targetFPS=144);
    float getDeltaTime();
}