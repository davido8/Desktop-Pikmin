#include <iostream>
#include <cstdlib>
#include <ctime>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_assert.h>
#include "onion.hpp"
#include <assert.h>

#include "spritesheet/spritesheet.hpp"

const char *onionImg = "sprites/onion_sheet.png";
const char *onionJson = "sprites/onion_data.json";

extern int screenWidth;
extern int screenHeight;

/*  Loads a texture specified in path into the rendering context. It sets the
    color [BLANK] as the transparent pixel, therefore, this color should not be
    used or it will be treated as transparent. */
static SDL_Texture *loadTexture(SDL_Window *window, SDL_Renderer *renderer, const char *path) {
    // Load image and enable color keying for transparent pixels.
    SDL_Surface *tempSurface = IMG_Load(path);
    SDL_Surface *optimizedSurface = SDL_ConvertSurface(tempSurface, 
                                                       SDL_GetWindowSurface(window)->format, 0);
    
    SDL_SetColorKey(optimizedSurface, SDL_TRUE, 
                    SDL_MapRGB(optimizedSurface->format, 0xFF, 0xFF, 0xFF));
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, optimizedSurface);
    SDL_FreeSurface(optimizedSurface);
    SDL_FreeSurface(tempSurface);

    return t;
}

SDL_Texture *Seed::seedTexture = nullptr;
SDL_Renderer *Seed::renderer = nullptr;
SDL_Window *Seed::window = nullptr;

int Seed::w = 8;
int Seed::h = 9;

const SDL_Rect Seed::seedFrames[6] = {
    {0,  0, 8, 10},
    {9,  0, 8, 10},
    {18, 0, 8, 10},
    {27, 0, 8, 10},
    {36, 0, 8, 10},
    {45, 0, 8, 10}
};

/* 
    Seed(): Constructor for the Seed class that places a seed into the flying
            state and chooses a final spot it will fly to.
*/
Seed::Seed(Onion *onion)
{
    state = Flying;

    int onionX = onion->getX();
    int onionY = onion->getY();
    int onionW = onion->getWidth();
    int onionH = onion->getHeight();

    x = onionX + (onionW / 2) - ((5 * w) / 2);
    y = onionY - 75;

    finalX = onionX + (rand() % ((onionW - (5*w))));
    finalY = onionY + onionH - (5 * h);

    this->onion = onion;
}

void Seed::doFrame() 
{
    int xVel, yVel, delay;
    bool draw = true;
    switch (state)
    {
        /* Move one step closer to final destination. */
        case Flying:
            xVel = SDL_abs(finalX - x) < 3 ? 1 : 3;
            xVel = x >= finalX ? -xVel : xVel; 
            yVel = finalY - y < 5 ? 1 : 5;

            x = x != finalX ? x + xVel : finalX;
            y = y != finalY ? y + yVel : finalY;

            if (x == finalX && y == finalY) {
                state = Grounded;
            }
            break;
        /* Randomly pick how long this seed will take to bloom. */
        case Grounded:
            delay = ((rand() % 10) + 5);    /* 5 + 0-10 seconds. */
            std::cout << "Going to take "<< delay << " seconds to bloom.\n";
            delay *= 1000;                  /* Convert ticks to seconds. */
            bloomTime = SDL_GetTicks64() + delay;
            state = Blooming;
            break;
        /* Wait until enough time has passed and create a Pikmin when ready. */
        case Blooming:
            if (bloomTime <= SDL_GetTicks64())
            {
                /* TODO: Create Pikmin. */
                std::cout << "Seed bloomed!\n";
                state = Bloomed;
            }
            frame--;
            break;
        /* Mark ourselves safe to delete. */
        case Bloomed:
            /* Don't remove from onion list due to race condition. */
            /* wait but theres only one seed executing at a time. */
            /* Don't use a lock to synchronize, too slow. */
            /* Onion will remove any bloomed seeds on its own. */
            draw = false;
            break;
        default:
            exit(EXIT_FAILURE);
    }

    /* Only draw if necessary. */
    if (draw)
    {
        SDL_Rect dest = {x, y, 5*w, 5*h};
        SDL_RenderCopy(renderer, seedTexture, &seedFrames[frame%6], &dest);
        frame++;        
    }
}

void Seed::initializeTextures(SDL_Window *gWindow, SDL_Renderer *gRenderer)
{
    seedTexture = loadTexture(gWindow, gRenderer, seedIMG);
    if (SDL_RenderCopy(gRenderer, seedTexture, NULL, NULL) != 0) {
        printf("Error: Could not load seed sprite. %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    window = gWindow;
    renderer = gRenderer;
}

enum SeedState Seed::getState() { return state; }

/*  Initializes data structures for the Onion. This includes the Onion struct
    itself, seeds array, designates a landing spot for the onion, size, and
    loads the textures for both the onion and its seeds. */
Onion::Onion(SDL_Window *window, SDL_Renderer *renderer)
{
    // TODO: Make sure onion is properly scaled.
    w = 0.10 * screenWidth;
    h = 0.15 * screenHeight;

    // Load the texture for the Onion.
    sprites = new SpriteSheet(window, renderer, onionImg, onionJson);

    this->window = window;
    this->renderer = renderer;
    launchOnion();
}

void Onion::launchOnion()
{
    // Designate a landing spot.
    finalX = rand() % (screenWidth - w);
    finalY = rand() % (screenHeight - h);

    x = finalX;
    y = 0;

    state = Launching;
    onionSpeed = 0.01*screenHeight + 1;

    std::cout << "Onion launching to (" << finalX << ", " << finalY << ")!\n";
}

/* Creates a new seed in the Onion and launches it. Must be landed. */
void Onion::launchSeed()
{
    if (state == Landed)
    {
        noSeeds++;
        seeds.push_back(new Seed(this));
        std::cout << noSeeds << " seeds in onion\n";
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

void Onion::doFrame()
{
    SDL_RenderClear(renderer);
    this->tick++;
    switch (state)
    {
        case Launching:
            // Keep moving down screen until reach target destination.
            if (y < finalY)
            {
                // Draw onion to the screen.
                sprites->drawSprite(x, y, 2);
                y = (y + onionSpeed) > finalY ? finalY : onionSpeed + y;
            }
            else 
            {
                std::cout << "Onion has landed!\n";
                SDL_assert(x == finalX);
                SDL_assert(y == finalY);

                sprites->drawSprite(x, y, 2);
                state = Unfolding;
            }
            break;
        case Unfolding: {
            bool incremented = false;
            if (true) {
                incremented = sprites->nextSprite();
            }

            sprites->drawSprite(x, y, 2);

            if (!incremented) {
                state = Landed;
            }
            break;
        }
        case Landed:
            // Draw onion to the screen.
            sprites->drawSprite(x, y, 2);

            // Update each seed.
            for (Seed *seed: seeds)
            {
                seed->doFrame();
            }

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