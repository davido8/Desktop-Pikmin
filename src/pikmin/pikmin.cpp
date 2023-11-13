#include "pikmin.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_assert.h>
#include <assert.h>

extern int screenWidth;
extern int screenHeight;

const char *pikminImg = "sprites/pikmin_sheet.png";
const char *pikminJson = "sprites/pikmin_data.json";

Pikmin::Pikmin(SDL_Window *window, SDL_Renderer *renderer, int x, int y)
{
    sprites = new SpriteSheet(window, renderer, pikminImg, pikminJson);

    // Start at the position given of the seed.
    this->x = x;
    this->y = y;

    // Scale appropriately.
    scale = 2;

    // Maintain widht/height to avoid going off screen.
    w = sprites->getWidth()*scale;
    h = sprites->getHeight()*scale;

    // Be ready to receive a task.
    state = Idle;
    speed = 10;

    // Face forward initially.
    sprites->setCurrent(Down*3);
}

enum PikminState Pikmin::chooseAction()
{
    int lucky = rand() % 100;
    if (lucky < 20) {
        return Resting;
    }

    return Meander;
}

void Pikmin::selectAction() 
{
    // Draw Pikmin.
    sprites->drawSprite(x, y, scale);

    // Randomly decide next action.
    enum PikminState state = chooseAction();
    switch (state) 
    {
        case Idle:
        case Resting:
            // Choose how long to rest.
            endActionTime = SDL_GetTicks64() + (rand() % 5)*1000;
            this->state = Resting;
            break;

        case Meander:
            // Choose how long to meander.
            endActionTime = SDL_GetTicks64() + (rand() % 5)*1000;
            this->state = Meander;

            // Pick direction to meander.
            direction = static_cast<Direction>(rand() % DCount);
            baseFrame = 3*static_cast<int>(direction);

            sprites->setCurrent(baseFrame);

            // Up = 0-2
            // Down = 3-5
            // Left = 6-8
            // Right = 9-11
            break;
        
        default:
            printf("PikminState %d not handled yet.\n", state);
            exit(EXIT_FAILURE);
    }
}

void Pikmin::nappyTime()
{
    sprites->drawSprite(x, y, scale);

    // Nap time done.
    if (endActionTime <= SDL_GetTicks64())
    {
        state = Idle;
    }
}

void Pikmin::handleWalking()
{
    sprites->drawSprite(x, y, scale);

    // Stop if at border.
    bool pastTop = y < 0 && direction == Up;
    bool pastBottom = y + h >= screenHeight && direction == Down;
    bool pastLeft = x < 0 && direction == Left;
    bool pastRight = x + w >= screenWidth && direction == Right;
    if (pastTop || pastBottom || pastLeft || pastRight) {
        state = Idle;
        return;
    }

    // Stop if our time is up.
    if (endActionTime <= SDL_GetTicks64()) {
        state = Idle;
        return;
    }

    // Only update position and frame every so often.
    if (SDL_GetTicks64() % 6 == 0) {
        // Update position for next draw.
        switch (direction)
        {
            case Up:
                y -= speed;
                break;
            case Down:
                y += speed;
                break;
            case Left:
                x -= speed;
                break;
            case Right:
                x += speed;
                break;
            default:
                printf("Error: Direction state not handled.\n");
                exit(EXIT_FAILURE);
        }

        // Update and reset frame to base if needed.
        bool incremented = sprites->nextSprite(false);
        if (sprites->getCurrentSprite() % 3 == 0 || !incremented) {
            sprites->setCurrent(baseFrame);
        }       
    }
}

void Pikmin::doFrame() 
{
    switch (state) 
    {
        case Idle:
            selectAction();
            break;
        case Resting:
            nappyTime();
            break;
        case Meander:
            handleWalking();
            break;
        default:
            exit(EXIT_FAILURE);
    }
}