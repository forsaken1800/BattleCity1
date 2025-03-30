#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

using namespace std;

class TextRenderer
{
    public:
        static void drawText(SDL_Renderer* render,
                            TTF_Font* font,
                            const string& message,
                            int x, int y,
                            SDL_Color color = {255, 255, 255, 255});
        virtual ~TextRenderer();

    protected:

    private:
};

#endif // TEXTRENDERER_H
