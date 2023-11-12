#ifndef SHEET_HPP
#define SHEET_HPP

#include "SDL.h"

class SpriteSheet {
    private:
        int spriteCount;
        int currentSprite;

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
        bool nextSprite();
        bool setCurrent(int);

        int getSpriteCount();
};

#endif