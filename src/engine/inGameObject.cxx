#include "inGameObject.hxx"
#include <SDL.h>
#include <SDL_keyboard.h>
#include <SDL_keycode.h>
#include "renderer.hxx"
#include <iostream>

inGameObject::inGameObject(float init_x, float init_y, int init_width,
    int init_height, float init_speed, SDL_Color init_objectColor,
    std::string init_objectTag, int init_objectInitalRenderCoordinateX,
    int init_objectInitalRenderCoordinateY) : 
        x(init_x), y(init_y), width(init_width), height(init_height),
        movementSpeed(init_speed), objectColor(init_objectColor),
        objectTag(init_objectTag), objectInitalRenderCoordinateX(init_objectInitalRenderCoordinateX),
        objectInitalRenderCoordinateY(init_objectInitalRenderCoordinateY)
        {}

inGameObject::inGameObject() : x(0.0), y(0.0), width(0), height(0), movementSpeed(0) {}
inGameObject::~inGameObject(){

}


void inGameObject::updateObjectState(float deltaTime,int levelWidth,int levelHeight) {
    const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
    bool isRunning;
    velocityX = 0.0;
    if (keyBinds->isActionPressed("move_left")){
        velocityX = -2.5f;
        if (animationStaterManagerClass.getCurrentAnimation()) {
            animationStaterManagerClass.getCurrentAnimation()->setFlipping(true); 
        }
    }
    if (keyBinds->isActionPressed("move_right")) {
        velocityX = 2.5f;
        if (animationStaterManagerClass.getCurrentAnimation()) {
            animationStaterManagerClass.getCurrentAnimation()->setFlipping(false); 
        }
    }
    isRunning = keyBinds->isActionPressed("run");
    if (isRunning) {
        velocityX *= 1.5f;
    }
    if (!isGrounded){
        velocityY += gravity * deltaTime;
    }
    y += velocityY * deltaTime;
    if(keyBinds->isActionPressed("jump")) jumpBufferTimer=jumpBufferGap;
    updateJumpBuffer(deltaTime);
    if ((isGrounded) && jumpBufferTimer>0) {
        velocityY = -800.0;
        isGrounded = false;
        jumpBufferTimer=0;
    }
    if(!keyBinds->isActionPressed("jump")&&velocityY<0.0) velocityY*=0.5;
    if (!isGrounded) {
        if (animationStaterManagerClass.getCurrentStateName() != "jump") {
            animationStaterManagerClass.play("jump");
        }
    }
    else if (velocityX != 0) {
        if (isRunning) {
            if (animationStaterManagerClass.getCurrentStateName() != "run") {
                animationStaterManagerClass.play("run");
            }
        } else {
            if (animationStaterManagerClass.getCurrentStateName() != "walk") {
                animationStaterManagerClass.play("walk");
            }
        }
    }
    else {
        if (animationStaterManagerClass.getCurrentStateName() != "idle") {
            animationStaterManagerClass.play("idle");
        }
    }
    animationStaterManagerClass.update(deltaTime);
    if (x < 0) x = 0;
    else if (x + width > levelWidth) x = levelWidth - width;
    x+=velocityX*movementSpeed*deltaTime;
}

void inGameObject::renderObject(Renderer* renderer, camera& cam, texture& textureClass) {
    float dstX = x - cam.getCameraX();
    float dstY = y - cam.getCameraY();
    float dstW = (float)width;
    float dstH = (float)height;

    if (animationStaterManagerClass.getCurrentAnimation()) {
        GLuint glTex = animationStaterManagerClass.getCurrentAnimation()->getTexture()->loadedTexture;
        SDL_Rect srcRect = animationStaterManagerClass.getCurrentAnimation()->getCurrentFrameRect();
        SDL_RendererFlip flip = animationStaterManagerClass.getCurrentAnimation()->getFlip();
        float renderScale = 1.0f;
        float xOffset = (width * renderScale - srcRect.w * renderScale) / 2.0f;
        float yOffset = srcRect.h * renderScale - height;
        if (objectTag == "Player") {
            dstX = x - cam.getCameraX() + xOffset;
            dstY = y - cam.getCameraY() - yOffset;
            dstW = (float)(srcRect.w * renderScale);
            dstH = (float)(srcRect.h * renderScale);
        }
        bool flipX = (flip == SDL_FLIP_HORIZONTAL);

        // compute UVs
        float u0 = (float)srcRect.x / animationStaterManagerClass.getCurrentAnimation()->getTexture()->textureWidth;
        float v0 = 1.0f - (float)(srcRect.y + srcRect.h) / animationStaterManagerClass.getCurrentAnimation()->getTexture()->textureHeight;
        float u1 = (float)(srcRect.x + srcRect.w) / animationStaterManagerClass.getCurrentAnimation()->getTexture()->textureWidth;
        float v1 = 1.0f - (float)srcRect.y / animationStaterManagerClass.getCurrentAnimation()->getTexture()->textureHeight;
        if (flipX) std::swap(u0, u1);

        renderer->submitQuad(glTex, dstX, dstY, dstW, dstH, u0, v0, u1, v1, 1.0f);
    }
    else if (objectTag == "Wall" || objectTag == "MovingPlatform") {
        int tileW = 18, tileH = 18;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                std::string tileKey;
                if (height == 1 && width == 1) {
                    tileKey = "SingleTop";
                } else if (height == 1) {
                    if (j == 0) tileKey = "SingleHTopLeft";
                    else if (j == width - 1) tileKey = "SingleHTopRight";
                    else tileKey = "SingleHTopMiddle";
                } else if (width == 1) {
                    if (i == 0) tileKey = "SingleVTop";
                    else if (i == height - 1) tileKey = "SingleVBottom";
                    else tileKey = "SingleVMiddle";
                } else {
                    if (i == 0) {
                        if (j == 0) tileKey = "MultiLineTopLeft";
                        else if (j == width - 1) tileKey = "MultiLineTopRight";
                        else tileKey = "MultiLineTopMiddle";
                    } else if (i == height - 1) {
                        if (j == 0) tileKey = "MultiLineLastLeft";
                        else if (j == width - 1) tileKey = "MultiLineLastRight";
                        else tileKey = "MultiLineLastMiddle";
                    } else {
                        if (j == 0) tileKey = "MultiLineMiddleLeft";
                        else if (j == width - 1) tileKey = "MultiLineMiddleRight";
                        else tileKey = "MultiLineMiddleMiddle";
                    }
                }

                auto tex = textureClass.getIndiviualTexture(tileKey);
                if (!tex) continue;

                float posX = x + j * tileW - cam.getCameraX();
                float posY = y + i * tileH - cam.getCameraY();

                float u0 = 0.0f, v0 = 0.0f, u1 = 1.0f, v1 = 1.0f;
                renderer->submitQuad(tex->loadedTexture, posX, posY, (float)tileW, (float)tileH,
                                     u0, v0, u1, v1, 1.0f);
            }
        }
    }
    else if (textureClass.getIndiviualTexture(objectTag)) {
        auto tex = textureClass.getIndiviualTexture(objectTag);
        int tileW = tex->textureWidth;
        int tileH = tex->textureHeight;
        for (int offsetY = 0; offsetY < height; offsetY += tileH) {
            for (int offsetX = 0; offsetX < width; offsetX += tileW) {
                float dstx = x + offsetX - cam.getCameraX();
                float dsty = y + offsetY - cam.getCameraY();
                float dw = (float)std::min(tileW, width - offsetX);
                float dh = (float)std::min(tileH, height - offsetY);

                float u0 = 0.0f, v0 = 0.0f, u1 = dw / (float)tex->textureWidth, v1 = dh / (float)tex->textureHeight;
                renderer->submitQuad(tex->loadedTexture, dstx, dsty, dw, dh, u0, v0, u1, v1, 1.0f);
            }
        }
    }
    else {
        // leave fillRect immediate for now (not batched)
        renderer->fillRect(dstX, dstY, dstW, dstH,
                           objectColor.r/255.0, objectColor.g/255.0,
                           objectColor.b/255.0, objectColor.a/255.0);
    }
}


float inGameObject::getX() { return x; }
float inGameObject::getY() { return y; }
std::string inGameObject::getObjectTag() { return objectTag; }
float inGameObject::getWidth() { return width; }
float inGameObject::getHeight() { return height; }
float inGameObject::getVelocityX() { return velocityX; }
float inGameObject::getVelocityY() { return velocityY; }
float inGameObject::getGrounded() { return isGrounded; }
int inGameObject::getObjectInitalRenderCoordinateX(){return objectInitalRenderCoordinateX;}
int inGameObject::getObjectInitalRenderCoordinateY(){return objectInitalRenderCoordinateY;}
void inGameObject::setX(float newX) { x = newX; }
void inGameObject::setY(float newY) { y = newY; }
void inGameObject::setVelocityX(float newVelocityX) {velocityX = newVelocityX;}
void inGameObject::addVelocityX(float newVelocityX) {velocityX += newVelocityX;}
void inGameObject::setVelocityY(float newVelocityY) {velocityY = newVelocityY;}
void inGameObject::setGrounded(bool newState) { isGrounded = newState; }
void inGameObject::setKeyBinds(keyBindManager& keyBindClass) {keyBinds=&keyBindClass;}
void inGameObject::updateJumpBuffer(float deltaTime){
    jumpBufferTimer-=deltaTime;
}
void inGameObject::setAsPlayer(bool val){ isPlayer = val; }
bool inGameObject::getIsPlayer(){ return isPlayer; }

void inGameObject::damage(int dmg){
    if(invulnTimer > 0.0f) return;
    health -= dmg;
    invulnTimer = 1.0f; // 1s i-frame
    if(health <= 0) {
        // TODO: trigger death animation/state
    }
}

void inGameObject::updateInvuln(float deltaTime){
    if(invulnTimer > 0.0f) invulnTimer -= deltaTime;
}