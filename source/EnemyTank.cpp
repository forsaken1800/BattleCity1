#include "EnemyTank.h"
#include "PlayerTank.h"
#include "Constants.h"
#include <algorithm>
#include <cmath>

EnemyTank::EnemyTank(SDL_Texture* sheet, int startX, int startY)
{
    spriteSheet = sheet;

    moveDelay = 15;
    shootDelay = 5;
    x = startX;
    y = startY;
    rect = { (int)x, (int)y, TILE_SIZE, TILE_SIZE };
    dirX = 0;
    dirY = 1;
    active = true;
    //ctor
}

void EnemyTank::move(const vector<Wall>& walls, const vector<Steel>& steels, const PlayerTank& player) {
    if (--moveDelay > 0) return;
    moveDelay = 15;

    int r = rand() % 4;
    if (r == 0) { // Up
        this->dirX = 0;  this->dirY = -5;
    } else if (r == 1) { // Down
        this->dirX = 0;  this->dirY = 5;
    } else if (r == 2) { // Left
        this->dirX = -5; this->dirY = 0;
    } else { // Right
        this->dirX = 5;  this->dirY = 0;
    }

    int newX = x + dirX;
    int newY = y + dirY;
    SDL_Rect newRect = { newX, newY, TILE_SIZE, TILE_SIZE };

    for (auto &wall : walls) {
        if (wall.active && SDL_HasIntersection(&newRect, &wall.rect)) {
            return;
        }
    }

    for (auto &steel : steels) {
        if (steel.active && SDL_HasIntersection(&newRect, &steel.rect)) {
            return;
        }
    }

    if (SDL_HasIntersection(&newRect, &player.rect)) {
        return;
    }

    if (newX >= TILE_SIZE && newX <= SCREEN_WIDTH - TILE_SIZE * 2 &&
        newY >= TILE_SIZE && newY <= SCREEN_HEIGHT - TILE_SIZE * 2) {
        x = newX;
        y = newY;
        rect.x = x;
        rect.y = y;
    }
}

void EnemyTank::shoot() {
    if (--shootDelay > 0) return;
    shootDelay = 5;
    bullets.push_back(Bullet(x + TILE_SIZE / 2 - 5,y + TILE_SIZE / 2 - 5,
                            this->dirX,this->dirY, spriteSheet));
    }

void EnemyTank::updateBullets() {
    for (auto &bullet : bullets) {
        bullet.move();
    }
    bullets.erase(remove_if(bullets.begin(), bullets.end(),
                                [](Bullet &b) { return !b.active; }),bullets.end());
}

static SDL_Rect tankGreen[4] = {
    {129,  130, 13, 13}, // UP
    {225,  129, 13, 13}, // RIGHT
    {193,  129, 13, 13}, // DOWN
    {162,  129, 13, 13}  // LEFT
};

void EnemyTank::render(SDL_Renderer* renderer) {
    if (!spriteSheet) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect rect = { x, y, 40, 40 };
        SDL_RenderFillRect(renderer, &rect);
        return;
    }

    int directionIndex = 0; // mặc định Up
    if (dirX == 0 && dirY < 0) {
        directionIndex = 0; // Up
    } else if (dirX > 0 && dirY == 0) {
        directionIndex = 1; // Right
    } else if (dirX == 0 && dirY > 0) {
        directionIndex = 2; // Down
    } else if (dirX < 0 && dirY == 0) {
        directionIndex = 3; // Left
    }

    SDL_Rect srcRect = tankGreen[directionIndex];

    SDL_Rect destRect = { x, y, 40, 40 };

    SDL_RenderCopy(renderer, spriteSheet, &srcRect, &destRect);

    for (auto &bullet : bullets) {
        bullet.render(renderer);
    }
}

EnemyTank::~EnemyTank()
{
    //dtor
}
