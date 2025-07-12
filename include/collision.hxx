#pragma once
#include <SDL.h>
#include "inGameObject.hxx"

namespace collision{
    bool checkAABB(inGameObject&,inGameObject&);
}