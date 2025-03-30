#include "Bullet.h"
#include "Constants.h"

Bullet::Bullet(int startX, int startY, int dirX, int dirY, SDL_Texture* sheet)
{
    x = startX;
    y = startY;
    dx = dirX;
    dy = dirY;
    active = true;
    rect = {x, y, 10, 10};
    spriteSheet = sheet;
    //ctor
}

void Bullet::move() {
    x += dx;
    y += dy;
    rect.x = x;
    rect.y = y;

    if (x < TILE_SIZE || x > SCREEN_WIDTH - TILE_SIZE ||
        y < TILE_SIZE || y > SCREEN_HEIGHT - TILE_SIZE) {
        active = false;
    }
}

static SDL_Rect bulletFrames[4] = {
    {323, 102, 3, 4},    // Up
    {346, 102, 4, 3},    // Right
    {339, 102, 3, 4},   // Down
    {330, 102, 4, 3}    // Left
};

void Bullet::render(SDL_Renderer* renderer) {
    if(!active) return;
    if (!spriteSheet) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &rect);
        return;
    }
    int directionIndex = 0;
        if (dx == 0 && dy < 0)
            directionIndex = 0; // Up
        else if (dx > 0 && dy == 0)
            directionIndex = 1; // Right
        else if (dx == 0 && dy > 0)
            directionIndex = 2; // Down
        else if (dx < 0 && dy == 0)
            directionIndex = 3; // Left

        SDL_Rect srcRect = bulletFrames[directionIndex];

        SDL_Rect destRect;

        if(directionIndex == 0 || directionIndex == 2) {
            destRect = { x, y, 6, 8 };
        }
        else destRect = { x, y, 8, 6 };

        SDL_RenderCopy(renderer, spriteSheet, &srcRect, &destRect);
}

Bullet::~Bullet()
{
    //dtor
}
