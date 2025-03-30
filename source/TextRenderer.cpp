#include "TextRenderer.h"
#include <iostream>

using namespace std;

void TextRenderer::drawText(SDL_Renderer* renderer,
                            TTF_Font* font,
                            const string& message,
                            int x, int y,
                            SDL_Color color)
{
    if (!renderer || !font) {
        cerr << "drawText: renderer or font is null!\n";
        return;
    }

    // Tạo surface từ text
    SDL_Surface* surf = TTF_RenderText_Solid(font, message.c_str(), color);
    if (!surf) {
        cerr << "TTF_RenderText_Solid error: " << TTF_GetError() << endl;
        return;
    }

    // Tạo texture từ surface
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    if (!tex) {
        cerr << "SDL_CreateTextureFromSurface error: " << SDL_GetError() << endl;
        return;
    }

    // Lấy kích thước text
    int w, h;
    if (TTF_SizeText(font, message.c_str(), &w, &h) == -1) {
        cerr << "TTF_SizeText error: " << TTF_GetError() << endl;
        SDL_DestroyTexture(tex);
        return;
    }

    // Vẽ text
    SDL_Rect destRect = { x, y, w, h };
    SDL_RenderCopy(renderer, tex, nullptr, &destRect);

    // Hủy texture
    SDL_DestroyTexture(tex);
    //ctor
}

TextRenderer::~TextRenderer()
{
    //dtor
}
