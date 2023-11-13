#include <iostream>
#include <cstdlib>
#include <ctime>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "onion.hpp"
#include "sounds.hpp"

#include <SDL_syswm.h>
#include <fileapi.h>
#include <windows.h>

#include "spritesheet.hpp"

#define MAKE_TRANSPARENT 1

int screenWidth;
int screenHeight;

// Makes a window transparent by setting a transparency color.
// https://stackoverflow.com/questions/23048993/sdl-fullscreen-translucent-background
bool MakeWindowTransparent(SDL_Window* window, COLORREF colorKey) {
    // Get window handle (https://stackoverflow.com/a/24118145/3357935)
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);  // Initialize wmInfo
    SDL_GetWindowWMInfo(window, &wmInfo);
    HWND hWnd = wmInfo.info.win.window;

    // Change window type to layered (https://stackoverflow.com/a/3970218/3357935)
    SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

    // Set transparency color
    return SetLayeredWindowAttributes(hWnd, colorKey, 0, LWA_COLORKEY);
}

/*  Initializes all SDL systems and data structures we need to start with.
    Returns: 0 on success. -1 on error. */
int InitSDL(SDL_Window **window, SDL_Renderer **renderer, Mix_Music **music) {
    int success = 0;

    // Initialize all subsystems we need.
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("Error initializing SDL: %s", SDL_GetError());
        success = -1;
    }
    if (!IMG_Init(IMG_INIT_PNG)) {
        printf("Error initializing image library: %s", SDL_GetError());
        success = -1;
    }
    //Initialize SDL_mixer
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0 ) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        success = -1;
    }

    if (success == 0) {
        // Create a window.
        SDL_DisplayMode DM;
        SDL_GetCurrentDisplayMode(0, &DM);
        auto Width = DM.w;
        auto Height = DM.h;

        Uint32 wflags = SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_BORDERLESS | SDL_WINDOW_SKIP_TASKBAR;// | SDL_WINDOW_FULLSCREEN_DESKTOP;
        *window = SDL_CreateWindow("Onion",
                                    -5000, -5000,
                                    Width, Height, wflags);
        if (!*window) {
            printf("Error opening window: %s", SDL_GetError());
            success = -1;
        }

        // Attach a renderer to the window.
        Uint32 rflags = SDL_RENDERER_ACCELERATED;
        *renderer = SDL_CreateRenderer(*window, -1, rflags);
        if (!*renderer) {
            printf("Error opening renderer: %s", SDL_GetError());
            success = -1;
        }

        SDL_SetRenderDrawBlendMode(*renderer, SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(*renderer, 255, 255, 254, 255);

        // All parts of window not filled with a color will be transparent.
        if (MAKE_TRANSPARENT) {
            MakeWindowTransparent(*window, RGB(255, 255, 254));
        }

        SDL_SetWindowPosition(*window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

        //Load music
        *music = Mix_LoadMUS("sounds/music.mp3");
        if(*music == NULL) {
            printf("Error: %s\n", Mix_GetError());
            success = -1;
        }
    }

    return success;
}

SoundEffects *loadSounds() 
{
    bool success = true;
    SoundEffects *sounds = new SoundEffects();
    success &= sounds->addSound(OnionSpit, "sounds/spit.mp3");
    success &= sounds->addSound(SeedLanding, "sounds/seed_landing.mp3");
    success &= sounds->addSound(SeedPlucked, "sounds/pluck.mp3");
    success &= sounds->addSound(PikminPikmin, "sounds/pikmin.mp3");
    success &= sounds->addSound(PikminTittai, "sounds/tittai.mp3");

    if (!success) {
        printf("Error: Could not load sounds.\n");
        exit(EXIT_FAILURE);
    }

    return sounds;
}

int main(int argc, char *argv[])
{
    // Initialize and create SDL data structures.
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    Mix_Music *music = NULL;
    if (InitSDL(&window, &renderer, &music) == -1) {
        return EXIT_FAILURE;
    }
    SDL_GetWindowSize(window, &screenWidth, &screenHeight);
    srand((unsigned) time(0));
    printf("Screen size: (%d, %d)\n", screenWidth, screenHeight);

    // Initialize sound effects.
    SoundEffects *sounds = loadSounds();

    Onion *onion = new Onion(window, renderer, sounds);

    const int targetFPS = 30;
    const Uint32 frameDelay = 1000 / targetFPS;
    Uint32 frameStart, frameTime;

    int frames = 0;
    Uint32 startTime = SDL_GetTicks();

    // Start the music.
    Mix_PlayMusic(music, -1);

    // Enter game loop.
    SDL_Event e;
    SDL_bool quit = SDL_FALSE;
    while (!quit) {
        frameStart = SDL_GetTicks64();
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = SDL_TRUE;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                for (int i = 0; i < 1000; i++) {
                    onion->launchSeed();
                }
            }
            else if (e.type == SDL_KEYDOWN) {
                // onion->doFrame();
            }
        }

        onion->doFrame();

        // Add delay to meet target frame rate.
        frameTime = SDL_GetTicks64() - frameStart;
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }
        frames++;

        // Calculate the time elapsed
        Uint32 currentTime = SDL_GetTicks();
        Uint32 elapsedTime = currentTime - startTime;

        // Update the frame rate every second
        if (elapsedTime >= 1000) {
            float fps = frames / (elapsedTime / 1000.0f);
            // std::cout << "FPS: " << fps << std::endl;

            // Reset counters
            frames = 0;
            startTime = currentTime;
        }
    }
    Mix_PauseMusic();

    Mix_FreeMusic(music);
    SDL_DestroyWindow(window);
    Mix_Quit();
    SDL_Quit();
    return 0;
}