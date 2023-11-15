
#ifndef ONION_HPP
#define ONION_HPP

#include <list>
#include "sprite.hpp"
#include "spritesheet.hpp"
#include "sounds.hpp"

constexpr const char *onionIMG = "sprites/onion.png";
constexpr const char *seedIMG = "sprites/seed.png";

class Onion;
class Seed;
class Pikmin;

enum OnionState {
    Launching, Unfolding, Landed
};

class Onion: Sprite {
    private:
        int finalX, finalY;         /* (x, y) coordinate once landed. */
        int onionSpeed;
        int scale;
        enum OnionState state;

        int noSeeds;                /* Number of seeds currently in the onion. */
        std::list<Seed *> seeds;    /* Current seeds in onion. */
        std::list<Pikmin *> pikmins;
        
        SDL_Window *window;
        SDL_Renderer *renderer;

        SpriteSheet *seedSprites;

        SoundEffects *soundBoard;

        void updatePosition();
        void extendLegs();
        void updateSeeds();
        void updatePikmin();
        void createPikmin(int x, int y);

        void clearSeeds();

    public:
        Onion(
            SDL_Window *window, 
            SDL_Renderer *renderer, 
            SoundEffects *soundBoard
        );
        
        void launchSeed();
        void doFrame();

        ~Onion();
};

#endif