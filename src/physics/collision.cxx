#include "collision.hxx"

namespace collision{
    bool checkAABB(inGameObject& a,inGameObject& b){
        return (a.getX()<b.getX()+b.getWidth() &&
                a.getX()+a.getWidth()>b.getX() &&
                a.getY()<b.getY()+b.getHeight() &&
                a.getY()+a.getHeight()>b.getY());
    }
}