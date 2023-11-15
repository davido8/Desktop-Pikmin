#include <iostream>
#include <cstdlib>
#include <ctime>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_assert.h>
#include "onion.hpp"
#include "seed.hpp"
#include "sprite.hpp"
#include "pikmin.hpp"
#include "sounds.hpp"
#include <assert.h>

#include <windows.h>

#include "spritesheet.hpp"

const char *onionImg = "assets/sprites/onion_sheet.png";
const char *onionJson = "assets/sprites/onion_data.json";

const char *seedImg = "assets/sprites/seed_sheet.png";
const char *seedJson = "assets/sprites/seed_data.json";

extern int screenWidth;
extern int screenHeight;

Uint64 tick;


/*  Initializes data structures for the Onion. This includes the Onion struct
    itself, seeds array, designates a landing spot for the onion, size, and
    loads the textures for both the onion and its seeds. */
Onion::Onion(
    SDL_Window *window, 
    SDL_Renderer *renderer, 
    SoundEffects *soundBoard): Sprite(window, renderer, "onion")
{
    this->setSprite("onion");

    // Load the sprite sheet containing all frames of Onion.
    // seedSprites = new SpriteSheet(window, renderer, seedImg, seedJson);

    // TODO: Add scale.
    scale = 2;

    // Store for drawing later.
    this->window = window;
    this->renderer = renderer;
    this->soundBoard = soundBoard;

    // TODO: Designate a landing spot.
    // finalX = rand() % (screenWidth - w);
    // finalY = rand() % (screenHeight - h);
    finalX = rand() % (screenWidth - this->getXBounds());
    finalY = rand() % (screenHeight - this->getYBounds());

    state = Launching;
    onionSpeed = 0.01*screenHeight + 1;

    this->move(finalX, 0);

    std::cout << "Onion launching to (" << finalX << ", " << finalY << ")!\n";
}

void Onion::createPikmin(int x, int y)
{
    soundBoard->playSound(SeedPlucked);
    pikmins.push_back(new Pikmin(window, renderer, soundBoard, x, y - 55));
}

/* Creates a new seed in the Onion and launches it. Must be landed. */
void Onion::launchSeed()
{
    if (state == Landed)
    {
        soundBoard->playSound(OnionSpit);
        seeds.push_back(new Seed(this, seedSprites, soundBoard));
        std::cout << seeds.size() << " seeds in onion\n";
    }
}

void Onion::clearSeeds()
{
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
    if (this->getY() >= finalY) {
        std::cout << "Onion has landed!\n";
        SDL_assert(this->getX() == finalX);
        SDL_assert(this->getY() == finalY);

        this->draw();
        state = Unfolding;
    }

    // Keep moving down screen until reach target destination.
    this->draw();
    this->move(this->getX(), (this->getY() + onionSpeed) > finalY ? finalY : onionSpeed + this->getY());
}

void Onion::extendLegs()
{
    bool incremented = this->nextFrame(false);
    this->draw();

    if (!incremented) {
        state = Landed;
        // launchSeed();
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
            this->draw();
            // updateSeeds();
            // updatePikmin();
            break;
        default:
            exit(EXIT_FAILURE);
    }
    SDL_RenderPresent(renderer);
}
