#include "PlayerTank.h"
#include "EnemyTank.h"
#include "Constants.h"
#include <algorithm>

PlayerTank::PlayerTank(SDL_Texture* sheet, int startX, int startY)
{
    x = startX;
    y = startY;
    rect = {x, y, TILE_SIZE, TILE_SIZE};
    dirX = 0.;
    dirY = -1;
    shootDelay = 0;
    //ctor
}

void PlayerTank::move(int dx, int dy, const vector<Wall>& walls, const vector<Steel>& steels, const vector<EnemyTank>& enemies) {
    int newX = x + dx;
    int newY = y + dy;
    this->dirX = dx;
    this->dirY = dy;

    SDL_Rect newRect = { newX, newY, TILE_SIZE, TILE_SIZE };
    for(size_t i = 0; i < walls.size(); i++) {
        if (walls[i].active && SDL_HasIntersection(&newRect, &walls[i].rect)) {
            return;
        }
    }

    for(size_t i = 0; i < steels.size(); i++) {
        if (steels[i].active && SDL_HasIntersection(&newRect, &steels[i].rect)) {
            return;
        }
    }

    for (auto &enemy : enemies) {
        if (SDL_HasIntersection(&newRect, &enemy.rect)) {
            return;
        }
    }

    if (newX >= TILE_SIZE && newX <= SCREEN_WIDTH - TILE_SIZE * 2 &&
        newY >= TILE_SIZE && newY <= SCREEN_HEIGHT - TILE_SIZE * 2) {
        x = newX;
        y = newY;
        rect.x = x;
        rect.y = y;
    }
}


static SDL_Rect tankYellow[4] = {
    {1,   2, 13, 13}, // UP
    {97,  1, 13, 13}, // RIGHT
    {65,  1, 13, 13}, // DOWN
    {34,  1, 13, 13}  // LEFT
};

void PlayerTank::shoot() {
    if (shootDelay > 0) {
        return;
    }

    shootDelay = 15;

    bullets.push_back(Bullet(x + TILE_SIZE / 2 - 5,y + TILE_SIZE / 2 - 5,
                            this->dirX,this->dirY, spriteSheet));
}

void PlayerTank::update() {
    if(shootDelay > 0) {
        shootDelay--;
    }
    updateBullets();
}

void PlayerTank::updateBullets() {
    for (auto &bullet : bullets) {
        bullet.move();
    }
    bullets.erase(remove_if(bullets.begin(), bullets.end(),
                                [](Bullet &b) { return !b.active; }),bullets.end());
}

void PlayerTank::render(SDL_Renderer* renderer){
    if (!spriteSheet) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect rect = { x, y, TILE_SIZE, TILE_SIZE };
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

    SDL_Rect srcRect = tankYellow[directionIndex];

    SDL_Rect destRect = { x, y, TILE_SIZE, TILE_SIZE };

    SDL_RenderCopy(renderer, spriteSheet, &srcRect, &destRect);

    for (auto &bullet : bullets) {
        bullet.render(renderer);
    }
}

PlayerTank::~PlayerTank()
{
    //dtor
}
