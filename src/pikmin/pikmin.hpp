#ifndef PIKMIN_HPP
#define PIKMIN_HPP

#include <SDL.h>
#include <SDL_image.h>

class Pikmin 
{
    int x, y;                   /* (x, y) coordinate on screen. */
    int finalX, finalY;         /* (x, y) coordinate once landed. */
    int w, h;                   /* Width and height of Onion. */

    SDL_Window *window;
    SDL_Texture *sprite;
    SDL_Renderer *renderer;

    public:
        Pikmin(SDL_Window *window, SDL_Renderer *renderer);

        void doFrame();
};

#endif