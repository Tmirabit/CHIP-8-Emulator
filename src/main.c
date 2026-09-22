#include <stdio.h>
#include <SDL3/SDL.h> // SDL graphics and input
//#include "chip8.h"

// chip8 myChip8;

int main(int argc, char**argv)
{
	if (!SDL_Init(SDL_INIT_VIDEO))
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "CHIP-8",
        640,
        320,
        0
    );

    if (window == NULL)
    {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    printf("SDL3 initialized successfully!\n");

    SDL_Delay(3000);

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
