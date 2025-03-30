#ifndef ENEMYTANK_H
#define ENEMYTANK_H

#include <SDL.h>
#include <vector>
#include "Bullet.h"
#include "Wall.h"
#include "Steel.h"

using namespace std;

class PlayerTank;

class EnemyTank
{
    public:
        SDL_Texture* spriteSheet;
        int x, y;
        int dirX, dirY;
        int moveDelay, shootDelay;
        SDL_Rect rect;
        bool active;
        vector<Bullet> bullets;

        EnemyTank(SDL_Texture* sheet, int startX, int startY);
        void move(const vector<Wall>& walls, const vector<Steel>& steels, const PlayerTank& player);
        void shoot();
        void updateBullets();
        void render(SDL_Renderer* renderer);

        virtual ~EnemyTank();

    protected:

    private:
};

#endif // ENEMYTANK_H
