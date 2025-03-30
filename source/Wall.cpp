#include "Wall.h"
#include "Constants.h"

SDL_Texture* Wall::spriteSheet = nullptr;

Wall::Wall(int startX, int startY)
{
    x = startX;
    y = startY;
    active = true;
    rect = {x, y, TILE_SIZE, TILE_SIZE};
    //ctor
}

void Wall::render(SDL_Renderer* renderer) {
    if (!active) return;

    if (!spriteSheet) {
        SDL_SetRenderDrawColor(renderer, 150, 75, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        return;
    }

    SDL_Rect srcRect = { 256, 0, 16, 16 };

    SDL_Rect destRect = { x, y, TILE_SIZE, TILE_SIZE };

    SDL_RenderCopy(renderer, spriteSheet, &srcRect, &destRect);
}

Wall::~Wall()
{
    //dtor
}
