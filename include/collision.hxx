#pragma once
#include <SDL.h>
#include "inGameObject.hxx"

namespace collision{
    bool checkAABB(inGameObject&,inGameObject&);
    void resolveCollision(inGameObject& player, inGameObject& surface);
    bool isTouchingGround(inGameObject& player, inGameObject& surface);
}