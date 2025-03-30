#ifndef BULLET_H
#define BULLET_H

#include <SDL.h>


class Bullet
{
    public:
        int x, y;
        int dx, dy;
        SDL_Rect rect;
        bool active;
        SDL_Texture* spriteSheet;

        Bullet(int startX, int startY, int dirX, int dirY, SDL_Texture* sheet);
        void move();
        void render(SDL_Renderer* renderer);
        virtual ~Bullet();
};

#endif // BULLET_H
