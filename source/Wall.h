#ifndef WALL_H
#define WALL_H

#include <SDL.h>

class Wall
{
    public:
        int x, y;
        SDL_Rect rect;
        bool active;
        static SDL_Texture* spriteSheet;

        Wall(int startX, int startY);
        virtual ~Wall();

        void render(SDL_Renderer* renderer);

    protected:

    private:
};

#endif // WALL_H
