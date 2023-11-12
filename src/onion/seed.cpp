#include <iostream>
#include <cstdlib>
#include <ctime>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_assert.h>
#include "onion.hpp"
#include "seed.hpp"
#include <assert.h>

#include "spritesheet/spritesheet.hpp"

/* 
    Seed(): Constructor for the Seed class that places a seed into the flying
            state and chooses a final spot it will fly to.
*/
Seed::Seed(Onion *onion, SpriteSheet *sheet)
{
    state = Flying;

    int onionX = onion->getX();
    int onionY = onion->getY();
    int onionW = onion->getWidth();
    int onionH = onion->getHeight();

    w = sheet->getWidth();
    h = sheet->getHeight();

    x = onionX + (onionW / 2) - ((5 * w) / 2);
    y = onionY - 75;

    finalX = onionX + (rand() % ((onionW - (5*w))));
    finalY = onionY + onionH - (5 * h);

    sprites = sheet;
    this->onion = onion;
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

void Seed::setBloomTime()
{
    int delay = ((rand() % 10) + 5);    /* 5 + 0-10 seconds. */
    std::cout << "Going to take "<< delay << " seconds to bloom.\n";
    delay *= 1000;                  /* Convert ticks to seconds. */
    bloomTime = SDL_GetTicks64() + delay;
    state = Blooming;
}

void Seed::waitToBloom()
{
    if (bloomTime <= SDL_GetTicks64())
    {
        /* TODO: Create Pikmin. */
        std::cout << "Seed bloomed!\n";
        state = Bloomed;
    }
}

void Seed::doFrame() 
{
    bool draw = true;
    bool freeze = false;
    tick++;
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
        if (tick % 3 == 0) {
            sprites->nextSprite(true);
        }
        sprites->drawSprite(x, y, 5);
    }
}

enum SeedState Seed::getState() { return state; }
