#include <iostream>
#include <SDL.h>
#include <vector>
#include <algorithm>
#include <ctime>
#include <Wall.h>
#include <Constants.h>
#include <Bullet.h>
#include <PlayerTank.h>
#include <EnemyTank.h>
#include <Game.h>

using namespace std;

int main(int agrc, char** argv) {
    srand((unsigned int)time(NULL));

    Game game;
    if(game.running) {
        game.run();
    }
    return 0;
}
