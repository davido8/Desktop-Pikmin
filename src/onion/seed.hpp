#ifndef SEED_HPP
#define SEED_HPP

#include "SDL.h"
#include "onion.hpp"

enum SeedState {
    Flying, Blooming, Grounded, Bloomed, Head
};

class Seed {
    private:
        int x, finalX;
        int y, finalY;
        Uint64 tick;
        int frame;
        Uint64 bloomTime;
        int w, h;

        enum SeedState state;

        Onion *onion;

        SDL_Window *window;
        SDL_Renderer *renderer;
        SpriteSheet *sprites;

        void updatePosition();
        void setBloomTime();
        void waitToBloom();

    public:
        Seed(Onion *onion, SpriteSheet *sheet);
        void doFrame();
        enum SeedState getState();
};

#endif