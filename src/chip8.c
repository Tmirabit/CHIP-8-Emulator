
#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char chip8_fontset[80] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void chip8_init(Chip8 *chip8)
{
	// Initialize registers and memory once
	chip8->pc     = 0x200;
	chip8->opcode = 0;
	chip8->I      = 0;
	chip8->sp     = 0;

	// Clear Display
	for(int i=0; i<sizeof(chip8->gfx)/sizeof(chip8->gfx[0]); ++i)
		chip8->gfx[i] = 0;

	// clear stack
	for(int i=0; i<sizeof(chip8->stack)/sizeof(chip8->stack[0]); ++i)
		chip8->stack[i] = 0;

	// Clear registers v0->VF
	for(int i=0; i<sizeof(chip8->V)/sizeof(chip8->V[0]); ++i)
		chip8->V[i] = 0;

	// Clear memory
	for(int i=0; i<sizeof(chip8->memory)/sizeof(chip8->memory[0]); ++i)
		chip8->memory[i] = 0;
	
	// Load Fontset
	for(int i=0; i<80; ++i)
		chip8->memory[0x50 + i] = chip8_fontset[i];

	// reset timers
	chip8->delay_timer = 0;
	chip8->sound_timer = 0;
	chip8->drawFlag = true;
}

void chip8_Cycle(Chip8 *chip8)
{
	// Fetch Opcode
	unsigned short opcode = (chip8->memory[chip8->pc] << 8) | 
							 chip8->memory[chip8->pc+1];
	unsigned char X = (opcode & 0x0F00) >> 8;
	unsigned char Y = (opcode & 0x00F0) >> 4;
	unsigned char N = opcode & 0x000F;
	unsigned char NN = opcode & 0x00FF;
	unsigned short NNN = opcode & 0x0FFF;

	// Decode Opcode
	switch(opcode & 0xF000)
	{
		case 0x0000:
			switch(opcode & 0x000F)
			{
				case 0x0000: // 0x00E0

					memset(chip8->gfx, 0, sizeof(chip8->gfx));
					chip8->drawFlag = true;
					chip8->pc += 2;
					break;

				case 0x000E: // 0x00EE
					if(chip8->sp > 0)
					{
						chip8->sp--;
						chip8->pc = chip8->stack[chip8->sp] + 2;
					}
					break;

				default:
					printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
			}
			break;
		case 0x1000: // 0x1NNN
			chip8->pc = NNN;
			break;
		case 0x2000: // 0x2NNN
			printf("CALL: PC=0x%04X -> 0x%03X, SP=%d\n",
				chip8->pc,
				NNN,
				chip8->sp);
			chip8->stack[chip8->sp] = chip8->pc;
			chip8->sp++;
			chip8->pc = NNN;
			break;
		case 0x3000: // 0x3XNN
			if (chip8->V[X] == NN)
				chip8->pc += 4;
			else
				chip8->pc += 2;
			break; 
		case 0x4000: // 0x4XNN
			if (chip8->V[X] != NN)
				chip8->pc += 4;
			else
				chip8->pc += 2;
			break;
		case 0x5000: // 0x5XY0
			if (chip8->V[X] == chip8->V[Y])
				chip8->pc += 4;
			else
				chip8->pc += 2;
			break;
		case 0x6000: // 0x6XNN
			chip8->V[X] = NN;
			chip8->pc += 2;
			break;
		case 0x7000: // 0x7XNN
			chip8->V[X] += NN;
			chip8->pc += 2;
			break;
		case 0x8000:
			switch(opcode & 0x000F)
			{
				case 0x0000: // 0x8XY0
					chip8->V[X] = chip8->V[Y];
					chip8->pc += 2;
					break;
				case 0x0001: // 0x8XY1
					chip8->V[X] |= chip8->V[Y];
					chip8->pc += 2;
					break;
				case 0x0002: // 0x8XY2
					chip8->V[X] &= chip8->V[Y];
					chip8->pc += 2;
					break;
				case 0x0003: // 0x8XY3
					chip8->V[X] ^= chip8->V[Y];
					chip8->pc += 2;
					break;
				case 0x0004: // 0x8XY4
					if(chip8->V[Y] > (0xFF - chip8->V[X]))
						chip8->V[0xF] = 1; // Carry
					else
						chip8->V[0xF] = 0;
					chip8->V[X] += chip8->V[Y];
					chip8->pc += 2;
					break;
				case 0x0005: // 0x8XY5
					if(chip8->V[X] >= chip8->V[Y])
						chip8->V[0xF] = 1;
					else
						chip8->V[0xF] = 0;
					chip8->V[X] -= chip8->V[Y];
					chip8->pc += 2;
					break;
				case 0x0006: // 0x8XY6
					chip8->V[0xF] = chip8->V[X] & 0x01;
					chip8->V[X] >>= 1;
					chip8->pc += 2;
					break;
				case 0x0007: // 0x8XY7
					if(chip8->V[Y] >= chip8->V[X])
						chip8->V[0xF] = 1;
					else
						chip8->V[0xF] = 0;
					chip8->V[X] = chip8->V[Y] - chip8->V[X];
					chip8->pc += 2;
					break;
				case 0x000E: // 0x8XYE
					if(chip8->V[X] & 0x10)
						chip8->V[0xF] = 1;
					else
						chip8->V[0xF] = 0;
					chip8->V[X] <<= 1;
					chip8->pc += 2;
					break;
				default:
					printf("Unknown opcode [0x8000]:  0x%X\n", opcode);
			}
			break;
		case 0x9000: // 0x9XY0
			if(chip8->V[X] != chip8->V[Y])
				chip8->pc += 4;
			else
				chip8->pc += 2;
			break;
		case 0xA000: // 0xANNN
			chip8->I = NNN;
			chip8->pc += 2;
			break;
		case 0xB000: // 0xBNNN
			chip8->pc = chip8->V[0] + NNN;
			break;
		case 0xC000: // 0xCXNN
			chip8->V[X] = (rand()%256) & NN;
			chip8->pc += 2;
			break;
		case 0xD000: // 0xDXYN
		{
			unsigned short pixel;

			chip8->V[0xF] = 0;
			for(int yline=0; yline<N; yline++)
			{
				pixel = chip8->memory[chip8->I + yline];
				for(int xline=0; xline<8; xline++)
				{
					if((pixel & (0x80>>xline)) != 0)
					{
						int x = (chip8->V[X] + xline) % 64;
						int y = (chip8->V[Y] + yline) % 32;
						int index = x + (y * 64);
						if(chip8->gfx[index] == 1)
							chip8->V[0xF] = 1;
						chip8->gfx[index] ^= 1;
					}
				}
			}
			chip8->drawFlag = true;
			chip8->pc += 2;
			break;
		}
		case 0xE000:
			switch(opcode & 0x000F)
			{
				case 0x000E: // 0xEX9E
					if(chip8->key[chip8->V[X]] != 0)
						chip8->pc += 4;
					else
						chip8->pc += 2;
					break;
				case 0x0001: // 0xEXA1
					if(chip8->key[chip8->V[X]] == 0)
						chip8->pc += 4;
					else
						chip8->pc += 2;
					break;
				default:
					printf("Unknown opcode [0xE000]:  0x%X\n", opcode);
			}
			break;
		case 0xF000:
			switch(opcode & 0x000F)
			{
				case 0x0007: // 0xFX07
					chip8->V[X] = chip8->delay_timer;
					chip8->pc += 2;
					break;
				case 0x000A: // 0xFX0A
					chip8->waiting_for_key = true;
					chip8->key_register = X;
					break;
				case 0x0008: // 0xFX18
					chip8->sound_timer = chip8->V[X];
					chip8->pc += 2;
					break;
				case 0x000E: // 0xFX1E
					chip8->I += chip8->V[X];
					chip8->pc += 2;
					break;
				case 0x0009: // 0xFX29
					chip8->I = 0x50 + (chip8->V[X] * 5);
					chip8->pc += 2;
					break;
				case 0x0003: // 0xFX33
					chip8->memory[chip8->I] = chip8->V[X] / 100;
					chip8->memory[chip8->I + 1] = (chip8->V[X] / 10) % 10;
					chip8->memory[chip8->I + 2] = (chip8->V[X] % 100) % 10;
					chip8->pc += 2;
					break;
				case 0x0005: 
					if((opcode & 0x00F0) == 0x0050){	
						// 0xFX55
						for(int i=0; i <= X; i++)
						{	
							chip8->memory[chip8->I + i] = chip8->V[i];
						}
						chip8->pc += 2;
					}else if((opcode &0x00F0) == 0x0010){	
						// 0xFX15
						chip8->delay_timer = chip8->V[X];
						chip8->pc += 2;
					}else{
						// 0xFX65
						for(int i=0; i <= X; i++)
						{
							chip8->V[i] = chip8->memory[chip8->I + i];
						}
						chip8->pc += 2;
					}
					break;
				default:					
					printf("Unknown opcode [0xF000]:  0x%X\n", opcode);
			}
			break;
		default:
			printf("Unknown opcode: 0x%X\n", opcode);
	}

	//Update timers
	if(chip8->delay_timer > 0)
	{
		--chip8->delay_timer;
	}
	if(chip8->sound_timer > 0)
	{
		if(chip8->sound_timer == 1)
			printf("BEEP!\n");
		--chip8->sound_timer;
	}
	
	// printf("PC: 0x%03X | Opcode: 0x%04X\n", chip8->pc, opcode);
}


bool chip8_load_game(Chip8 *chip8, const char *filename)
{
    FILE *file = fopen(filename, "rb");

    if (file == NULL)
    {
        printf("Failed to open ROM: %s\n", filename);
        return false;
    }

    size_t bytes_read = fread(
        chip8->memory + 0x200,
        1,
        sizeof(chip8->memory) - 0x200,
        file
    );

    fclose(file);

    printf("Loaded ROM: %s (%zu bytes)\n", filename, bytes_read);

    return true;
}
