#ifndef SEED_HPP
#define SEED_HPP

#include "SDL.h"
#include "onion.hpp"
#include "sounds.hpp"

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
        SoundEffects *soundBoard;


        void updatePosition();
        void setBloomTime();
        void waitToBloom();

    public:
        Seed(Onion *onion, SpriteSheet *sheet, SoundEffects *soundBoard);
        void doFrame();
        enum SeedState getState();
        int getX();
        int getY();
};

#endif