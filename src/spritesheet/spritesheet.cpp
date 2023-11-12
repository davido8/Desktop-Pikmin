#include "spritesheet.hpp"
#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_assert.h>
#include <assert.h>

#include <iostream>
#include <fstream>

#include "json.hpp"
using json = nlohmann::json;

/*  Loads a texture specified in path into the rendering context. It sets the
    color [BLANK] as the transparent pixel, therefore, this color should not be
    used or it will be treated as transparent. */
static SDL_Texture *loadTexture(SDL_Window *window, SDL_Renderer *renderer, const char *path) {
    SDL_Surface *tempSurface = IMG_Load(path);
    if (!tempSurface) {
        printf("SDL_Image Error: %s\n", IMG_GetError());
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    if (!texture) {
        printf("SDL_Image Error: %s\n", IMG_GetError());
        return nullptr;
    }

    return texture;
}

SpriteSheet::SpriteSheet(
    SDL_Window *window,
    SDL_Renderer *renderer,
    const char *imgPath, 
    const char *jsonPath
) 
{
    printf("Going to load in spritesheet %s\n", imgPath);

    this->window = window;
    this->renderer = renderer;

    // Open the JSON file
    std::ifstream file(jsonPath);

    // Check if the file is open
    if (!file.is_open()) {
        std::cout << "Failed to open the file!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Parse the JSON data
    json jsonData;
    file >> jsonData;

    std::cout << "Frames: " << jsonData["frames"].size() << " \n";

    currentSprite = 0;
    spriteCount = jsonData["frames"].size();
    frames = new SDL_Rect[spriteCount];

    int i = 0;
    for (const auto &sprite: jsonData["frames"]) {
        frames[i].x = sprite["frame"]["x"].get<int>();
        frames[i].y = sprite["frame"]["y"].get<int>();
        frames[i].w = sprite["frame"]["w"].get<int>();
        frames[i].h = sprite["frame"]["h"].get<int>();
        i++;
    }

    spritesheet = loadTexture(window, renderer, imgPath);

    file.close();
}

void SpriteSheet::drawSprite(int x, int y, int scale) 
{
    SDL_assert(currentSprite < spriteCount);
    SDL_Rect *src = &frames[currentSprite];
    SDL_Rect dest = {x, y, src->w*scale, src->h*scale};
    SDL_RenderCopy(renderer, spritesheet, src, &dest);
}

/*
    Increments to the next frame and returns true if succeeded. Returns false
    if at last frame.
*/
bool SpriteSheet::nextSprite() 
{ 
    bool success = currentSprite < (spriteCount - 1);
    if (success) {
        currentSprite++;
    }
    return success;
}

bool SpriteSheet::setCurrent(int frame)
{
    if (frame < spriteCount) {
        currentSprite = frame;
        return true;
    }

    return false;
}

int SpriteSheet::getSpriteCount() { return spriteCount; }