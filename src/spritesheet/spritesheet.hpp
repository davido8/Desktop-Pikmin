#ifndef SHEET_HPP
#define SHEET_HPP

#include "SDL.h"

class SpriteSheet {
    private:
        int spriteCount;
        int currentSprite;

        int width;
        int height;

        SDL_Rect *frames;
        SDL_Texture *spritesheet;
        SDL_Window *window;
        SDL_Renderer *renderer;

    public:
        SpriteSheet(
            SDL_Window *window, 
            SDL_Renderer *renderer,
            const char *imgPath, 
            const char *jsonPath
        );

        void drawSprite(int x, int y, int scale);
        bool nextSprite(bool);
        bool setCurrent(int);

        int getSpriteCount();
        int getWidth();
        int getHeight();
};

#endif