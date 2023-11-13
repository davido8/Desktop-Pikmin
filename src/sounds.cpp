#include "sounds.hpp"
#include <iostream>
#include <map>

SoundEffects::SoundEffects() {}

bool SoundEffects::addSound(enum Sounds sound, char *path) 
{
    sounds[sound] = Mix_LoadWAV(path);
    if (sounds[sound] == NULL) {
        printf("Error: Could not load sound %s\n", path);
        return false;
    }

    return true;
}

bool SoundEffects::playSound(enum Sounds sound)
{
    if (sounds[sound] == NULL) {
        return false;
    }

    Mix_PlayChannel(-1, sounds[sound], 0);
    return true;
}