#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <SDL.h>
#include <vector>

using namespace std;

class Explosion
{
    public:
        int x, y;
        SDL_Rect destRect; // Vị trí và kích thước khi vẽ
        vector<SDL_Rect> frames; // Các frame của animation
        int currentFrame; // Frame hiện tại
        int frameDelay;   // Số frame delay giữa 2 frame animation
        int counter;      // Bộ đếm để chuyển frame
        bool finished;// Hiệu ứng kết thúc

        Explosion(int x, int y, int width, int height, const vector<SDL_Rect>& frames, int frameDelay = 5);
        void update();
        void render(SDL_Renderer* renderer, SDL_Texture* spriteSheet);

        virtual ~Explosion();

    protected:

    private:
};

#endif // EXPLOSION_H
