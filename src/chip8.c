
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

	// Decode Opcode
	switch(opcode & 0xF000)
	{
		case 0x0000:
			switch(opcode & 0x000F)
			{
				case 0x0000: // 0x00E0
				break;

				case 0x000E: // 0x00EE
				break;

				default:
					printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
			}
		case 0x1000: // 0x1NNN
			break;
		case 0x2000: // 0x2NNN
			break;
		case 0x3000: // 0x3XNN
			break;
		case 0x4000: // 0x4XNN
			break;
		case 0x5000: // 0x5XY0
			break;
		case 0x6000: // 0x6XNN
			break;
		case 0x7000: // 0x7XNN
			break;
		case 0x8000:
			switch(opcode & 0x000F)
			{
				case 0x0000: // 0x8XY0
					break;
				case 0x0001: // 0x8XY1
					break;
				case 0x0002: // 0x8XY2
					break;
				case 0x0003: // 0x8XY3
					break;
				case 0x0004: // 0x8XY4
					break;
				case 0x0005: // 0x8XY5
					break;
				case 0x0006: // 0x8XY6
					break;
				case 0x0007: // 0x8XY7
					break;
				case 0x000E: // 0x8XYE
					break;
				default:
					printf("Unknown opcode [0x8000]:  0x%X\n", opcode);
			}
		case 0x9000: // 0x9XY0
			break;
		case 0xA000: // 0xANNN
			chip8->I = opcode & 0x0FFF;
			chip8->pc += 2;
			break;
		case 0xB000: // 0xBNNN
			chip8->pc = chip8->V[0] + (opcode & 0x0FFF);
			break;
		case 0xC000: // 0xCXNN
			break;
		case 0xD000: // 0xDXYN
			break;
		case 0xE000:
			switch(opcode & 0x000F)
			{
				case 0x000E: // 0xEX9E
					break;
				case 0x0001: // 0xEXA1
					break;
				default:
					printf("Unknown opcode [0xE000]:  0x%X\n", opcode);
			}
		case 0xF000:
			switch(opcode & 0x000F)
			{
				case 0x0007: // 0xFX07
					break;
				case 0x000A: // 0xFX0A
					break;
				case 0x0008: // 0xFX18
					break;
				case 0x000E: // 0xFX1E
					break;
				case 0x0009: // 0xFX29
					break;
				case 0x0003: // 0xFX33
					break;
				case 0x0005: 
					if((opcode & 0x00F0) == 0x0050){	
						// 0xFX55
					}else if((opcode &0x00F0) == 0x0010){	
						// 0xFX15
					}else{
						// 0xFX65
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



