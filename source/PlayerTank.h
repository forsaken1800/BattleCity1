#ifndef PLAYERTANK_H
#define PLAYERTANK_H

#include <vector>
#include <SDL.h>
#include "Bullet.h"
#include "Wall.h"
#include "Steel.h"

using namespace std;

class EnemyTank;

class PlayerTank
{
    public:
        SDL_Texture* spriteSheet;
        int x, y;
        int dirX, dirY;
        int shootDelay;
        SDL_Rect rect;
        vector<Bullet> bullets;

        PlayerTank (SDL_Texture* sheet, int startX, int startY);

        void move(int dx, int dy, const vector<Wall>& walls, const vector<Steel>& steels, const vector<EnemyTank>& enemies);
        void shoot();
        void update();
        void updateBullets();
        void render(SDL_Renderer* renderer);

        virtual ~PlayerTank();

    protected:

    private:
};

#endif // PLAYERTANK_H
