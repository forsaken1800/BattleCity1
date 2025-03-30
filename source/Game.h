#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include "Wall.h"
#include "Steel.h"
#include "EnemyTank.h"
#include "PlayerTank.h"
#include "Explosion.h"

using namespace std;

enum GameState {
    MENU,
    PLAYING,
    WIN,
    GAMEOVER,
};

class Game
{
    public:
        GameState gameState;
        SDL_Window* window;
        SDL_Renderer* renderer;
        bool running;
        vector<Wall> walls;
        vector<Steel> steels;
        SDL_Texture* spriteSheet;
        PlayerTank player;
        int enemyNumber = 4;
        vector<EnemyTank> enemies;
        TTF_Font* font;
        Mix_Music* backgroundMusic;
        vector<Explosion> explosions;
        Uint32 countdownTime = 30000; // 30 giây
        Uint32 startTime = SDL_GetTicks();
        Uint32 elapsedTime;
        int secondsLeft;

        Game();
        void generateMap();
        void spawnEnemies();
        void update();
        void handleEvents();
        void render();
        void run();

        virtual ~Game();

    protected:

    private:
};

#endif // GAME_H
