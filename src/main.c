#include <stdio.h>
#include <stdint.h>
#include <SDL3/SDL.h> // SDL graphics and input

#include "chip8.h"

int get_chip8_key(SDL_Keycode key)
{
    switch (key)
    {
        case SDLK_1: return 0x1;
        case SDLK_2: return 0x2;
        case SDLK_3: return 0x3;
        case SDLK_4: return 0xC;

        case SDLK_Q: return 0x4;
        case SDLK_W: return 0x5;
        case SDLK_E: return 0x6;
        case SDLK_R: return 0xD;

        case SDLK_A: return 0x7;
        case SDLK_S: return 0x8;
        case SDLK_D: return 0x9;
        case SDLK_F: return 0xE;

        case SDLK_Z: return 0xA;
        case SDLK_X: return 0x0;
        case SDLK_C: return 0xB;
        case SDLK_V: return 0xF;

        default: return -1;
    }
}

int main(int argc, char**argv)
{

	bool running = true;
	uint32_t pixels[64 * 32];
	
	Chip8 chip8;	
	chip8_init(&chip8);
	
	 if (!chip8_load_game(&chip8, "Roms/Space Invaders [David Winter].ch8"))
    {
        return 1;
    }
	
	if (!SDL_Init(SDL_INIT_VIDEO))
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "CHIP-8 Emulator",
        640,
        320,
        0
    );

	SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

	SDL_Texture *texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STREAMING,
		64,
		32
	);
	
    if (window == NULL)
    {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

	while(running)
	{
		SDL_Event event;

		while(SDL_PollEvent(&event))
		{
			 switch (event.type)
			{
				case SDL_EVENT_QUIT:
					running = false;
					break;

				case SDL_EVENT_KEY_DOWN:
				{
					int key = get_chip8_key(event.key.key);

					if (key != -1)
					{
						chip8.key[key] = 1;

						// FX0A: waiting for a key press
						if (chip8.waiting_for_key)
						{
							chip8.V[chip8.key_register] = key;
							chip8.waiting_for_key = false;
							chip8.pc += 2;
						}
					}

					break;
				}

				case SDL_EVENT_KEY_UP:
				{
					int key = get_chip8_key(event.key.key);

					if (key != -1)
					{
						chip8.key[key] = 0;
					}

					break;
				}
			}
		}
		
		if (!chip8.waiting_for_key)
			chip8_Cycle(&chip8);

		if(chip8.drawFlag)
		{
			for (int i=0; i<64*32; i++)
			{
				if (chip8.gfx[i])
					pixels[i]=0xFFFFFFFF;
				else
					pixels[i]=0x000000FF;
			}

			SDL_UpdateTexture(
				texture,
				NULL,
				pixels,
				64 * sizeof(uint32_t)
			);

			SDL_RenderClear(renderer);
			SDL_RenderTexture(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);

			chip8.drawFlag = false;
		}
		
		SDL_Delay(2);
	}

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
