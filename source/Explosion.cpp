#include "Explosion.h"

Explosion::Explosion(int x, int y, int width, int height, const vector<SDL_Rect>& frames, int frameDelay)
    : x(x), y(y), frames(frames), frameDelay(frameDelay)
{
    currentFrame = 0;
    counter = 0;
    finished = false;
    destRect.x = x;
    destRect.y = y;
    destRect.w = width;
    destRect.h = height;
}

void Explosion::update() {
    if (finished) return;

    counter++;
    if (counter >= frameDelay) {
        counter = 0;
        currentFrame++;
        if ((size_t)currentFrame >= frames.size()) {
            finished = true;
        }
    }
}

void Explosion::render(SDL_Renderer* renderer, SDL_Texture* spriteSheet) {
    if (finished) return;
    SDL_RenderCopy(renderer, spriteSheet, &frames[currentFrame], &destRect);
}

Explosion::~Explosion()
{
    //dtor
}
