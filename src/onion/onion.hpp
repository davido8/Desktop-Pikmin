
#ifndef ONION_HPP
#define ONION_HPP

#include <list>
#include "spritesheet/spritesheet.hpp"

constexpr const char *onionIMG = "sprites/onion.png";
constexpr const char *seedIMG = "sprites/seed.png";

class Onion;
class Seed;

enum OnionState {
    Launching, Unfolding, Landed
};

class Onion 
{
    private:
        int x, y;                   /* (x, y) coordinate on screen. */
        int finalX, finalY;         /* (x, y) coordinate once landed. */
        int onionSpeed;
        int w, h;                   /* Width and height of Onion. */
        int scale;
        enum OnionState state;

        int noSeeds;                /* Number of seeds currently in the onion. */
        std::list<Seed *> seeds;  /* Current seeds in onion. */
        
        SDL_Window *window;
        SDL_Renderer *renderer;
        SpriteSheet *sprites;
        SpriteSheet *seedSprites;

        void updatePosition();
        void extendLegs();
        void updateSeeds();
        void createPikmin(int x, int y);

        void clearSeeds();

    public:
        Onion(SDL_Window *window, SDL_Renderer *renderer);
        
        void launchSeed();
        void doFrame();

        int getX();
        int getY();
        int getWidth();
        int getHeight();

        ~Onion();
};

#endif