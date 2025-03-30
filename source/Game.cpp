#include "Game.h"
#include "Constants.h"
#include "SDL.h"
#include "TextRenderer.h"
#include "Explosion.h"
#include "Bullet.h"
#include "Map.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <algorithm>

using namespace std;

Game::Game() : player(nullptr, ((MAP_WIDTH - 1) / 2) * TILE_SIZE, (MAP_HEIGHT - 2) * TILE_SIZE)
{
    gameState = MENU;
    running = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        running = false;
    }
    window = SDL_CreateWindow("Battle City",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH, SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
    if (!window) {
        cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        running = false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        running = false;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        cerr << "SDL_image could not initialize! " << IMG_GetError() << endl;
        running = false;
    }

    SDL_Surface* tempSurface = IMG_Load("NES - Battle City JPN - General Sprites.png");
    if (!tempSurface) {
        cerr << "Failed to load sprite: " << IMG_GetError() << endl;
        running = false;
    } else {
        spriteSheet = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);
        if (!spriteSheet) {
            cerr << "Failed to create texture: " << SDL_GetError() << endl;
            running = false;
        }
    }

    if (TTF_Init() == -1) {
        cerr << "TTF_Init error: " << TTF_GetError() << endl;
    }
    font = TTF_OpenFont("TIMES.TTF", 32);
    if (!font) {
        cerr << "TTF_OpenFont error: " << TTF_GetError() << endl;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cerr << "SDL_mixer could not initialize! " << Mix_GetError() << endl;
        running = false;
    }

    backgroundMusic = Mix_LoadMUS("y2mate.com - MAESTRO THEME  REMIX RUMBLE MUSIC  TFT SET 10.mp3");
    if (!backgroundMusic) {
        cerr << "Failed to load background music! " << Mix_GetError() << endl;
    } else {
    // Phát nhạc nền, lặp vô hạn (tham số -1)
        if (Mix_PlayMusic(backgroundMusic, -1) == -1) {
            cerr << "Failed to play music! " << Mix_GetError() << endl;
        }
    }

    player.spriteSheet = spriteSheet;
    Wall::spriteSheet = this->spriteSheet;
    Steel::spriteSheet = this->spriteSheet;
    generateMap();
    spawnEnemies();
    //ctor
}

void Game::generateMap() {
    vector<vector<int>> mapData = loadMapFromCSV("MAP2.csv");

    for (size_t row = 0; row < mapData.size(); row++) {
        for (size_t col = 0; col < mapData[row].size(); col++) {
            int tileID = mapData[row][col];
            int px = (col - 0) * TILE_SIZE;
            int py = (row - 0) * TILE_SIZE;

            switch (tileID) {
                case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:
                    break;
                case 8:
                    walls.push_back(Wall(px, py));
                    break;
                case 9:
                    steels.push_back(Steel(px, py));
                    break;
                default:
                    break;
            }
        }
    }
}

void Game::spawnEnemies() {
    enemies.clear();
    for (int i = 0; i < enemyNumber; ++i) {
        int ex, ey;
        bool validPosition = false;
        while (!validPosition) {
            ex = (rand() % (MAP_WIDTH - 2) + 1) * TILE_SIZE;
            ey = (rand() % (MAP_HEIGHT - 2) + 1) * TILE_SIZE;
            validPosition = true;
            for (const auto &wall : walls) {
                if (wall.active && wall.x == ex && wall.y == ey) {
                    validPosition = false;
                    break;
                }
            }
            for (const auto &steel : steels) {
                if (steel.active && steel.x == ex && steel.y == ey) {
                    validPosition = false;
                    break;
                }
            }
            for (const auto &enemy : enemies) {
                if (enemy.x == ex && enemy.y == ey) {
                    validPosition = false;
                    break;
                }
            }
            if (player.x == ex && player.y == ey) {
                validPosition = false;
            }
        }
        enemies.push_back(EnemyTank(spriteSheet, ex, ey));
    }
}

void Game::update() {
    player.update();

    Uint32 currentTime = SDL_GetTicks();
    Uint32 elapsedTime = currentTime - startTime;
    Uint32 remainingTime = (elapsedTime >= countdownTime) ? 0 : countdownTime - elapsedTime;
    secondsLeft = remainingTime / 1000;

    if (remainingTime == 0) {
        gameState = GAMEOVER;
    }

    for (auto& enemy : enemies) {
        enemy.move(walls, steels, player);
        enemy.updateBullets();
        if (rand() % 100 < 10) {
            enemy.shoot();
        }
    }

    for (auto& enemy : enemies) {
        for (auto &bullet : enemy.bullets) {
            for (auto &wall : walls) {
                if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                    wall.active = false;
                    bullet.active = false;
                    vector<SDL_Rect> explosionFrames = {
                    {256, 128, 16, 16},  // Ví dụ: frame 1 của hiệu ứng nổ (điều chỉnh toạ độ theo sprite sheet của bạn)
                    {272, 128, 16, 16}  // frame 4
                };
                explosions.push_back(Explosion(wall.x, wall.y, TILE_SIZE - 5, TILE_SIZE - 5, explosionFrames, 5));
                    break;
                }
            }
        }
    }

    for (auto& enemy : enemies) {
        for (auto &bullet : enemy.bullets) {
            for (auto &steel : steels) {
                if (steel.active && SDL_HasIntersection(&bullet.rect, &steel.rect)) {
                    bullet.active = false;
                }
            }
        }
    }

    for (auto &bullet : player.bullets) {
        for (auto &steel : steels) {
            if (steel.active && SDL_HasIntersection(&bullet.rect, &steel.rect)) {
                bullet.active = false;
            }
        }
    }

    for (auto &bullet : player.bullets) {
        for (auto &wall : walls) {
            if (wall.active && SDL_HasIntersection(&bullet.rect, &wall.rect)) {
                wall.active = false;
                bullet.active = false;
                vector<SDL_Rect> explosionFrames = {
                    {256, 128, 16, 16},  // Ví dụ: frame 1 của hiệu ứng nổ (điều chỉnh toạ độ theo sprite sheet của bạn)
                    {272, 128, 16, 16}  // frame 4
                };
                explosions.push_back(Explosion(wall.x, wall.y, TILE_SIZE - 5, TILE_SIZE - 5, explosionFrames, 5));
                break;
            }
        }
    }

    for (auto &pBullet : player.bullets) {
        for (auto &enemy : enemies) {
            for (auto &eBullet : enemy.bullets) {
                if (SDL_HasIntersection(&pBullet.rect, &eBullet.rect)) {
                    pBullet.active = false;
                    eBullet.active = false;
                }
            }
        }
    }

    for (auto &bullet : player.bullets) {
        for (auto& enemy : enemies) {
            if (enemy.active && SDL_HasIntersection(&bullet.rect, &enemy.rect)) {
                enemy.active = false;
                bullet.active = false;
                vector<SDL_Rect> explosionFrames = {
                    {256, 128, 16, 16},  // Ví dụ: frame 1 của hiệu ứng nổ (điều chỉnh toạ độ theo sprite sheet của bạn)
                    {272, 128, 16, 16},  // frame 2
                    {304, 128, 32, 32},  // frame 3
                    {336, 128, 32, 32}   // frame 4
                };
                explosions.push_back(Explosion(enemy.x, enemy.y, TILE_SIZE, TILE_SIZE, explosionFrames, 5));
            }
        }
    }

    enemies.erase(remove_if(enemies.begin(), enemies.end(),
                            [](EnemyTank &e) { return !e.active; }), enemies.end());

    if (enemies.empty()) {
        gameState = WIN;
    }

    for (auto &enemy : enemies) {
        for (auto &bullet : enemy.bullets) {
            if (SDL_HasIntersection(&bullet.rect, &player.rect)) {
                gameState = GAMEOVER;
                return;
            }
        }
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        else if (event.type == SDL_KEYDOWN) {
            if (gameState == MENU) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    gameState = PLAYING;
                    startTime = SDL_GetTicks();
                }
            }

            else if (gameState == PLAYING) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        player.move(0, -5, walls, steels, enemies);
                        break;
                    case SDLK_DOWN:
                        player.move(0, 5, walls, steels, enemies);
                        break;
                    case SDLK_LEFT:
                        player.move(-5, 0, walls, steels, enemies);
                        break;
                    case SDLK_RIGHT:
                        player.move(5, 0, walls, steels, enemies);
                        break;
                    case SDLK_SPACE:
                        player.shoot();
                        break;
                }
            }
            else if(gameState == GAMEOVER || gameState == WIN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    running = false;
                }
            }
        }
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderClear(renderer);

    if (gameState == MENU) {
        TextRenderer::drawText(renderer, font, "BATTLE CITY", 320, 200);
        TextRenderer::drawText(renderer, font, "Press ENTER to start", 280, 280);
    }

    else if (gameState == PLAYING) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        string timerText = "Time Left: " + to_string(secondsLeft);
        TextRenderer::drawText(renderer, font, timerText, 10, 10);

        for (int i = 1; i < MAP_HEIGHT - 1; ++i) {
            for (int j = 1; j < MAP_WIDTH - 1; ++j) {
                SDL_Rect tile = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                SDL_RenderFillRect(renderer, &tile);
            }
        }

        for(size_t i = 0; i < walls.size(); i++) {
            walls[i].render(renderer);
        }

        for(size_t i = 0; i < steels.size(); i++) {
            steels[i].render(renderer);
        }

        player.render(renderer);

        for (auto &enemy : enemies) {
            enemy.render(renderer);
        }

        for (auto it = explosions.begin(); it != explosions.end(); ) {
            it->update();
            it->render(renderer, spriteSheet);
            if (it->finished) {
                it = explosions.erase(it);
            } else {
                ++it;
            }
        }
    }

    else if (gameState == WIN) {
        TextRenderer::drawText(renderer, this->font, "YOU WIN!", 340, 280);
    }

    else if (gameState == GAMEOVER) {
        TextRenderer::drawText(renderer, this->font, "GAME OVER", 320, 280);
    }

    SDL_RenderPresent(renderer);
}

void Game::run() {
    while(running) {
        handleEvents();
        if (gameState == PLAYING) {
            update();
        }
        render();
        SDL_Delay(16);
    }
}

Game::~Game()
{
    SDL_DestroyTexture(spriteSheet);
    IMG_Quit();
    if (font) TTF_CloseFont(font);
    TTF_Quit();
    if (backgroundMusic) {
        Mix_FreeMusic(backgroundMusic);
        backgroundMusic = nullptr;
    }
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    //dtor
}
