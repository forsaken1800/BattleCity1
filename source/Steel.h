#ifndef STEEL_H
#define STEEL_H

#include <SDL.h>

class Steel
{
    public:
        int x, y;
        SDL_Rect rect;
        bool active;
        static SDL_Texture* spriteSheet;

        Steel(int startX, int startY);
        virtual ~Steel();

        void render(SDL_Renderer* renderer);


    protected:

    private:
};

#endif // STEEL_H
