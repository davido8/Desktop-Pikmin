#ifndef SOUNDS_HPP
#define SOUNDS_HPP

#include <map>
#include <SDL_mixer.h>

enum Sounds {
    SeedLanding, SeedPlucked, PikminPikmin
};

class SoundEffects {
    private:
        std::map<enum Sounds, Mix_Chunk *> sounds;

    public:
        SoundEffects();

        bool addSound(enum Sounds, char *path);
        bool playSound(enum Sounds);
};

#endif