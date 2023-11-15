#include "sprite.hpp"
#include "spritesheet.hpp"

#include "SDL.h"
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <filesystem>
#include <sys/stat.h>

const std::string jsonRoot   = "assets/sprites/json/";
const std::string sheetsRoot = "assets/sprites/sheets/";

/*
    TODO Safety issues:
        - Make sure we only do JSON files and PNG.
        - Make sure files are named correctly (x.json / x.png)
        - Still can never be completely safe... Have some faith.
*/
static std::map<std::string, std::string> findFiles(const std::string &base) {
    // Contains mapping from JSON -> Sheets or nullptr if none exists.
    std::map<std::string, std::string> pairs;

    std::vector<std::string> jsonFiles;

    // Find all JSON files.
    for (const auto& entry : std::filesystem::directory_iterator(jsonRoot)) {
 
        std::filesystem::path filePath = entry.path();
        std::string fullName = filePath.string();

        std::string fileName = fullName.substr(jsonRoot.length());
        if (fileName.find(base) != std::string::npos) {
            jsonFiles.push_back(fullName);
        }
    }

    // Add files with both data and sheets to map ONLY. Ignore incompletes.
    for (const auto& entry : std::filesystem::directory_iterator(sheetsRoot)) {
 
        std::filesystem::path filePath = entry.path();
        std::string fullName = filePath.string();
        std::string fileName = fullName.substr(sheetsRoot.length());

        // Make sure this is a sprite we want.
        if (fileName.find(base) != std::string::npos) {

            // Okay, it is, now see if we have a matching json file.
            for (std::string json: jsonFiles) {
                std::string jsonNoJSON = json.substr(json.find(base));
                jsonNoJSON = jsonNoJSON.substr(0, jsonNoJSON.find('.'));

                std::string fileNoPNG = fileName.substr(0, fileName.find('.'));

                if (jsonNoJSON == fileNoPNG) {
                    pairs[json] = fullName;
                }
            }
        }
    }

    return pairs;
}

/*
    Takes in a string which is the base of the files it will read in and create
    spritesheets for all of them. 
*/
Sprite::Sprite(
    SDL_Window *window,
    SDL_Renderer *renderer,
    const std::string name
) {
    std::cout << "Attempting to read in all " << name << " files in " << jsonRoot << "\n";

    // Load in all the sprites we need.
    std::map<std::string, std::string> files = findFiles(name);
    for (auto const& [json, sheet]: files) {
        std::string fileName = sheet.substr(sheetsRoot.length());
        fileName = fileName.substr(0, fileName.find('.'));
        sprites[fileName] = new SpriteSheet(window, renderer, sheet, json);
    }

    // x, y, w, h all left uninitialized until calls to draw.
    x = y = w = h = -1;

    this->currentSheet = nullptr;
    this->window = window;
    this->renderer = renderer;
}

void Sprite::move(int x, int y) {
    this->x = x;
    this->y = y;
}

void Sprite::draw() {
    SDL_assert(currentSheet != nullptr);
    currentSheet->drawSprite(x, y, 3);
}

bool Sprite::setSprite(std::string name) {
    if (sprites.find(name) == sprites.end()) {
        return false;
    }

    // Change the current sheet and update the bounds information.
    currentSheet = sprites[name];

    w = currentSheet->getWidth();
    h = currentSheet->getHeight();

    return true;
}

bool Sprite::nextFrame(bool reset) {
    return currentSheet->nextSprite(reset);
}


int Sprite::getX() { return x; }
int Sprite::getY() { return y; }
int Sprite::getXBounds() { return x + w; }
int Sprite::getYBounds() { return y + h; }