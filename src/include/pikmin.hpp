#ifndef PIKMIN_HPP
#define PIKMIN_HPP

#include <SDL.h>
#include <SDL_image.h>

#include "spritesheet.hpp"
#include "sounds.hpp"

enum Direction {
    Up, Down, Left, Right, DCount
};

enum PikminState {
    Idle, Resting, Meander, Count
};

class Pikmin 
{
    private:
        int x, y;                   /* (x, y) coordinate on screen. */
        int w, h;         
        int speed;
        int scale;
        Uint64 endActionTime;

        enum PikminState state;   

        // Variables needed for walking.
        int baseFrame;
        enum Direction direction;       

        SpriteSheet *sprites;
        SoundEffects *soundBoard;

        enum PikminState chooseAction();
        void selectAction();
        void nappyTime();
        void handleWalking();

    public:
        Pikmin(SDL_Window *window, SDL_Renderer *renderer, SoundEffects *soundBoard, int x, int y);

        void doFrame();
};

#endif