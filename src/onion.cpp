#include <iostream>
#include <cstdlib>
#include <ctime>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_assert.h>
#include "onion.hpp"
#include "seed.hpp"
#include "pikmin.hpp"
#include "sounds.hpp"
#include <assert.h>

#include <windows.h>

#include "spritesheet.hpp"

const char *onionImg = "sprites/onion_sheet.png";
const char *onionJson = "sprites/onion_data.json";

const char *seedImg = "sprites/seed_sheet.png";
const char *seedJson = "sprites/seed_data.json";

extern int screenWidth;
extern int screenHeight;

Uint64 tick;


/*  Initializes data structures for the Onion. This includes the Onion struct
    itself, seeds array, designates a landing spot for the onion, size, and
    loads the textures for both the onion and its seeds. */
Onion::Onion(SDL_Window *window, SDL_Renderer *renderer, SoundEffects *soundBoard)
{
    // Load the sprite sheet containing all frames of Onion.
    sprites = new SpriteSheet(window, renderer, onionImg, onionJson);
    seedSprites = new SpriteSheet(window, renderer, seedImg, seedJson);

    // TODO: Add scale.
    scale = 2;
    w = scale*sprites->getWidth();
    h = scale*sprites->getHeight();

    // Store for drawing later.
    this->window = window;
    this->renderer = renderer;
    this->soundBoard = soundBoard;

    // Designate a landing spot.
    finalX = rand() % (screenWidth - w);
    finalY = rand() % (screenHeight - h);

    x = finalX;
    y = 0;

    state = Launching;
    onionSpeed = 0.01*screenHeight + 1;

    std::cout << "Onion launching to (" << finalX << ", " << finalY << ")!\n";
}

void Onion::createPikmin(int x, int y)
{
    soundBoard->playSound(SeedPlucked);
    pikmins.push_back(new Pikmin(window, renderer, soundBoard, x, y));
}

/* Creates a new seed in the Onion and launches it. Must be landed. */
void Onion::launchSeed()
{
    if (state == Landed)
    {
        seeds.push_back(new Seed(this, seedSprites, soundBoard));
        std::cout << seeds.size() << " seeds in onion\n";
    }
}

void Onion::clearSeeds()
{
    /* TODO: Fix. */
    for (auto it = seeds.begin(); it != seeds.end();)
    {
        if ((*it)->getState() == Bloomed)
        {
            it = seeds.erase(it);
            delete *it;
        }
        else
        {
            it++;
        }
    }
}

void Onion::updatePosition() 
{
    if (y >= finalY) {
        std::cout << "Onion has landed!\n";
        SDL_assert(x == finalX);
        SDL_assert(y == finalY);

        sprites->drawSprite(x, y, scale);
        state = Unfolding;
    }

    // Keep moving down screen until reach target destination.
    sprites->drawSprite(x, y, scale);
    y = (y + onionSpeed) > finalY ? finalY : onionSpeed + y;
}

void Onion::extendLegs()
{
    bool incremented = sprites->nextSprite(false);

    sprites->drawSprite(x, y, scale);

    if (!incremented) {
        state = Landed;
        launchSeed();
    }
}

void Onion::updateSeeds()
{
    // Advance the seed's frame in here to make sure it doesn't happen once
    // per seed.
    if (tick % 6 == 0) {
        seedSprites->nextSprite(true);
    }

    std::list<Seed *>::iterator it = seeds.begin(); 
  
    while (it != seeds.end()) { 
        Seed *seed = *it;
        seed->doFrame();
        if (seed->getState() == Bloomed) { 
            createPikmin(seed->getX(), seed->getY());
            it = seeds.erase(it); 
            delete seed;
            continue; 
        } 
        it++; 
    } 
}

void Onion::updatePikmin()
{
    for (Pikmin *pikmin: pikmins) {
        pikmin->doFrame();
    }
}

void Onion::doFrame()
{
    SDL_RenderClear(renderer);
    tick++;
    switch (state)
    {
        case Launching:
            updatePosition();
            break;
        case Unfolding:
            extendLegs();
            break;
        case Landed:
            sprites->drawSprite(x, y, scale);
            updateSeeds();
            updatePikmin();
            break;
        default:
            exit(EXIT_FAILURE);
    }
    SDL_RenderPresent(renderer);
}

int Onion::getX() { return x; }
int Onion::getY() { return y; }
int Onion::getWidth() { return w; }
int Onion::getHeight() { return h; }