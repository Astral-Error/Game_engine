#include "shadowManager.hxx"
#include <glad/glad.h>
#include <cmath>

shadowManager::shadowManager(){
    globalDirX=-1.0;
    globalDirY = 1.0;
}

void shadowManager::setGlobalLight(float init_globalDirX, float init_globalDirY){
    globalDirX=init_globalDirX;
    globalDirY=init_globalDirY;
}

void shadowManager::renderGlobalShadow(const std::vector<inGameObject*>& gameObjects, camera& cam){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0,0.0,0.0,0.5); //translucent black

    glBegin(GL_QUADS);
    for(inGameObject* object : gameObjects){
        float x = object->getX()-cam.getCameraX(),
        y = object->getY()-cam.getCameraY(),
        w = object->getWidth(),
        h = object->getHeight();

        float offX = globalDirX*50,
        offY = globalDirY*50;

        glVertex2f(x,y+h);
        glVertex2f(x+w,y+h);
        glVertex2f(x+offX, y+h+offY);
        glVertex2f(x+w+offX, y+h+offY);
    }
    glEnd();
    glDisable(GL_BLEND);
}


void shadowManager::renderPointLightShadows(const std::vector<inGameObject*>& gameObjects, std::vector<LightSource>& lights,camera& cam){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for(LightSource& light: lights){
        for(inGameObject* obj : gameObjects){
            float x = obj->getX();
            float y = obj->getY();
            float w = obj->getWidth();
            float h = obj->getHeight();

            float cx = x + w / 2.0f;
            float cy = y + h / 2.0f;

            float dx = cx - light.x;
            float dy = cy - light.y;
            float len = std::sqrt(dx*dx + dy*dy);
            if (len == 0) continue;
            dx /= len; dy /= len;

            float offX = dx * light.radius;
            float offY = dy * light.radius;

            float rx = x - cam.getCameraX();
            float ry = y - cam.getCameraY();

            glColor4f(0.0f, 0.0f, 0.0f, 0.4f * light.intensity);
            glBegin(GL_QUADS);
            glVertex2f(rx, ry + h);
            glVertex2f(rx + w, ry + h);
            glVertex2f(rx + w + offX, ry + h + offY);
            glVertex2f(rx + offX, ry + h + offY);
            glEnd();
        }
    }
    glDisable(GL_BLEND);
}