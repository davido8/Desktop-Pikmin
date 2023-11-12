#include <iostream>
#include <cstdlib>
#include <ctime>

#include <SDL.h>
#include <SDL_image.h>
#include "pikmin.hpp"

#include <SDL_syswm.h>
#include <fileapi.h>
#include <windows.h>

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
int InitSDL(SDL_Window **window, SDL_Renderer **renderer) {
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

    if (success == 0) {
        // Create a window.
        Uint32 wflags = SDL_WINDOW_ALWAYS_ON_TOP;
        wflags = MAKE_TRANSPARENT ? wflags | SDL_WINDOW_FULLSCREEN_DESKTOP : wflags;
        *window = SDL_CreateWindow("Onion",
                                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    1920, 1080, wflags);
        if (!*window) {
            printf("Error opening window: %s", SDL_GetError());
            success = -1;
        }

        // Attach a renderer to the window.
        Uint32 rflags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
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
    }

    return success;
}

int main(int argc, char *argv[])
{
    // Initialize and create SDL data structures.
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    if (InitSDL(&window, &renderer) == -1) {
        return EXIT_FAILURE;
    }
    SDL_GetWindowSize(window, &screenWidth, &screenHeight);
    srand((unsigned) time(0));

    int x = 500;
    int y = 500;
    // Extract starting coordinates if supplied.
    if (argc == 3) {
        x = atoi(argv[1]);
        y = atoi(argv[2]);
    }

    Pikmin *pikmin = new Pikmin(window, renderer, x, y);

    const int targetFPS = 60;
    const Uint32 frameDelay = 1000 / targetFPS;
    Uint32 frameStart, frameTime;

    int frames = 0;
    Uint32 startTime = SDL_GetTicks();
     // Enter game loop.
    SDL_Event e;
    SDL_bool quit = SDL_FALSE;
    while (!quit) {
        frameStart = SDL_GetTicks64();
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = SDL_TRUE;
            }
            else if (e.type == SDL_KEYDOWN) {
                // pikmin->doFrame();
            }
        }

        pikmin->doFrame();

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
            std::cout << "FPS: " << fps << std::endl;

            // Reset counters
            frames = 0;
            startTime = currentTime;
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}