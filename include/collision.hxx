#pragma once
#include <SDL.h>
#include "inGameObject.hxx"
#include "texture.hxx"

namespace collision{
    bool checkAABB(inGameObject&, inGameObject&);
    bool checkAABB(inGameObject& a, inGameObject& b, texture& textureClass);
    void resolveCollision(inGameObject& player, inGameObject& surface);
    void resolveCollision(inGameObject& player, inGameObject& surface, texture& textureClass);
    bool isTouchingGround(inGameObject& player, inGameObject& surface);
    bool isTouchingGround(inGameObject& player, inGameObject& surface, texture& textureClass);
}