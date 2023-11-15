#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "spritesheet.hpp"
#include <map>
#include <string>

class Sprite {
    private:
        // Position of this sprite on the screen.
        int x, y;

        // Dimensions of this sprite.
        int w, h;


        // Map of spritesheets this sprite has for different animations.
        SpriteSheet *currentSheet;
        std::map<std::string, SpriteSheet *> sprites;

        SDL_Window *window;
        SDL_Renderer *renderer;
    public:
        Sprite(
            SDL_Window *window, 
            SDL_Renderer *renderer,
            const std::string name
        );

        // Used to calculate collisions.
        int getX();
        int getY();
        int getXBounds();
        int getYBounds();

        // Used to animate/move.
        void move(int x, int y);
        void draw();

        // Used to manage frames/sprites.
        bool nextFrame(bool);
        bool setSprite(std::string sprite);


};

#endif