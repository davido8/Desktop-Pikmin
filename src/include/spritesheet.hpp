#ifndef SHEET_HPP
#define SHEET_HPP

#include "SDL.h"
#include <string>

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
            const std::string &imgPath, 
            const std::string &jsonPath
        );

        void drawSprite(int x, int y, int scale);
        bool nextSprite(bool);
        bool setCurrent(int);

        int getSpriteCount();
        int getWidth();
        int getHeight();
        int getCurrentSprite();
};

#endif