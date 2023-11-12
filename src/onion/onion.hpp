
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

enum SeedState {
    Flying, Blooming, Grounded, Bloomed, Head
};

class Seed {
    int x, finalX;
    int y, finalY;
    int frame;
    Uint64 bloomTime;

    enum SeedState state;

    Onion *onion;

    static int w, h;
    static SDL_Window *window;
    static SDL_Texture *seedTexture;
    static SDL_Renderer *renderer;
    
    public:
        Seed(Onion *onion);
        void doFrame();
        enum SeedState getState();

        static void initializeTextures(SDL_Window *window, SDL_Renderer *renderer);
        static const SDL_Rect seedFrames[6];
};

class Onion 
{
    int x, y;                   /* (x, y) coordinate on screen. */
    int finalX, finalY;         /* (x, y) coordinate once landed. */
    int onionSpeed;
    int w, h;                   /* Width and height of Onion. */
    enum OnionState state;
    Uint64 tick;

    int noSeeds;                /* Number of seeds currently in the onion. */
    std::list<Seed *> seeds;  /* Current seeds in onion. */
    
    SDL_Window *window;
    SDL_Renderer *renderer;
    SpriteSheet *sprites;

    void clearSeeds();

    public:
        Onion(SDL_Window *window, SDL_Renderer *renderer);
        
        void launchOnion();
        void launchSeed();
        void doFrame();

        int getX();
        int getY();
        int getWidth();
        int getHeight();

        ~Onion();
};

#endif