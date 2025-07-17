#include "collision.hxx"

namespace collision{
    bool checkAABB(inGameObject& a,inGameObject& b){
        return (a.getX()<b.getX()+b.getWidth() &&
                a.getX()+a.getWidth()>b.getX() &&
                a.getY()<b.getY()+b.getHeight() &&
                a.getY()+a.getHeight()>b.getY());
    }

    void resolveCollision(inGameObject& player, inGameObject& surface){
        float playerLeft   = player.getX();
        float playerRight  = playerLeft + player.getWidth();
        float playerTop    = player.getY();
        float playerBottom = playerTop + player.getHeight();

        float platLeft   = surface.getX();
        float platRight  = platLeft + surface.getWidth();
        float platTop    = surface.getY();
        float platBottom = platTop + surface.getHeight();

        float overlapX = std::min(playerRight, platRight) - std::max(playerLeft, platLeft);
        float overlapY = std::min(playerBottom, platBottom) - std::max(playerTop, platTop);

        if (overlapX <= 0 || overlapY <= 0) return;

        if (player.getVelocityY() >= 0 && playerBottom - overlapY <= platTop){
            player.setY(platTop - player.getHeight());
            player.setVelocityY(0);
            player.setGrounded(true);
        }
        else if (player.getVelocityY() < 0 && playerTop + overlapY >= platBottom){
            player.setY(platBottom);
            player.setVelocityY(0);
        }
        else{
            if(playerRight - overlapX <= platLeft){
                player.setX(platLeft - player.getWidth());
            }
            else if(playerLeft + overlapX >= platRight){
                player.setX(platRight);
            }
        }
    }
    
    bool isTouchingGround(inGameObject& player, inGameObject& surface) {
        float threshold = 5.0f;
        float playerBottom = player.getY() + player.getHeight();
        float surfaceTop = surface.getY();

        bool verticallyAligned = playerBottom <= surfaceTop + threshold && playerBottom >= surfaceTop;
        bool horizontallyOverlapping = player.getX() + player.getWidth() > surface.getX() &&
                                    player.getX() < surface.getX() + surface.getWidth();

        return verticallyAligned && horizontallyOverlapping;    
    }
}