#ifndef SOUNDS_HPP
#define SOUNDS_HPP

#include <map>
#include <SDL_mixer.h>

enum Sounds {
    OnionSpit, SeedLanding, SeedPlucked, PikminPikmin, PikminTittai
};

class SoundEffects {
    private:
        std::map<enum Sounds, Mix_Chunk *> sounds;

    public:
        SoundEffects();

        bool addSound(enum Sounds, const char *path);
        bool playSound(enum Sounds);
};

#endif