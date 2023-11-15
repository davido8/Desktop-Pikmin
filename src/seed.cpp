#include <iostream>
#include <cstdlib>
#include <ctime>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_assert.h>
#include "onion.hpp"
#include "seed.hpp"
#include <assert.h>

#include "spritesheet.hpp"
#include "sounds.hpp"

extern Uint64 tick;

/* 
    Seed(): Constructor for the Seed class that places a seed into the flying
            state and chooses a final spot it will fly to.
*/
Seed::Seed(Onion *onion, SpriteSheet *sheet, SoundEffects *soundBoard)
{
    state = Flying;

    int onionX = 0;//onion->getX();
    int onionY = 0;//onion->getY();
    int onionW = 0;//onion->getWidth();
    int onionH = 0;//onion->getHeight();

    w = sheet->getWidth();
    h = sheet->getHeight();

    x = onionX + (onionW / 2) - ((5 * w) / 2);
    y = onionY - 75;

    finalX = onionX + (rand() % ((onionW - (5*w))));
    finalY = onionY + onionH - (5 * h);

    sprites = sheet;
    this->onion = onion;
    this->soundBoard = soundBoard;
}

void Seed::updatePosition()
{
    int xVel, yVel;

    xVel = SDL_abs(finalX - x) < 3 ? 1 : 3;
    xVel = x >= finalX ? -xVel : xVel; 
    yVel = finalY - y < 5 ? 1 : 5;

    x = x != finalX ? x + xVel : finalX;
    y = y != finalY ? y + yVel : finalY;

    if (x == finalX && y == finalY) {
        state = Grounded;
    }
}

/*
    Landed! Determine how long to wait before plucking.
*/
void Seed::setBloomTime()
{
    int delay = ((rand() % 10) + 5);    /* 5 + 0-10 seconds. */
    delay *= 1000;                  /* Convert ticks to seconds. */
    bloomTime = SDL_GetTicks64() + delay;
    soundBoard->playSound(SeedLanding);
    state = Blooming;
}

void Seed::waitToBloom()
{
    if (bloomTime <= SDL_GetTicks64())
    {
        state = Bloomed;
    }
}

void Seed::doFrame() 
{
    bool draw = true;
    bool freeze = false;
    switch (state)
    {
        case Flying:
            /* Move one step closer to final destination. */
            updatePosition();
            break;
        case Grounded:
            /* Randomly pick how long this seed will take to bloom. */
            setBloomTime();
            break;
        case Blooming:
            /* Wait until enough time has passed and create a Pikmin when ready. */
            waitToBloom();
            freeze = true;
            break;
        case Bloomed:
            /* Mark ourselves safe to delete. */
            draw = false;
            break;
        default:
            exit(EXIT_FAILURE);
    }

    /* Only draw if necessary. */
    if (draw) {
        /* Only advance frame every couple of ticks to prevent it going fast. */

        sprites->drawSprite(x, y, 5);
    }
}

enum SeedState Seed::getState() { return state; }
int Seed::getX() { return x; }
int Seed::getY() { return y; }