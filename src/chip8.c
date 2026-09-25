
#include "chip8.h"

#include <stdio.h>

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
		//chip8->memory[i] = chip8_fontset[i];

	// reset timers
	chip8->delay_timer = 0;
	chip8->sound_timer = 0;
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
					break;

				case 0x000E: // 0x00EE
					return;

				default:
					printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
			}
		case 0x1000: // 0x1NNN
			chip8->pc = NNN;
			break;
		case 0x2000: // 0x2NNN
			chip8->stack[chip8->sp] = chip8->pc;
			++chip8->sp;
			chip8->pc = NNN;
			break;
		case 0x3000: // 0x3XNN
			if (chip8->V[X] == NN)
				chip8->pc += 4;
			else
				chip8->pc += 2;
			break; 
		case 0x4000: // 0x4XNN
			if (chip8->V[X] != NN);
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
					chip8->V[X] |= chip8V[Y];
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
					if(chip8->V[Y] > (0xFF - V[X])
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
			// Display Code to fix
			break;
		case 0xE000:
			switch(opcode & 0x000F)
			{
				case 0x000E: // 0xEX9E
					// Key Press
					break;
				case 0x0001: // 0xEXA1
					// Key Press
					break;
				default:
					printf("Unknown opcode [0xE000]:  0x%X\n", opcode);
			}
		case 0xF000:
			switch(opcode & 0x000F)
			{
				case 0x0007: // 0xFX07
					chip8->V[X] = chip8->delay_timer;
					chip8->pc += 2;
					break;
				case 0x000A: // 0xFX0A
					// Key Press Code
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
					// Sprite font thing 
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
							chip8->memory[chip8->I + i] = chip8=>V[i];
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
}



