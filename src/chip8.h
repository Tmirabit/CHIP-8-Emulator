// chip8.h
#ifndef CHIP8_H
#define CHIP8_H

#include <stdbool.h>

typedef struct{
	
	unsigned short opcode; 
	unsigned char memory[4096]; 
	unsigned char V[16];			// CPU registers 
	unsigned short I;				// Index register
	unsigned short pc;				// Program counter

	unsigned char gfx[64 * 32];		// Graphics

	unsigned char delay_timer;
	unsigned char sound_timer;

	unsigned short stack[16];
	unsigned short sp;

	unsigned char key[16];
	
	bool waiting_for_key;
	unsigned char key_register;

	bool drawFlag;
	
} Chip8;

// Initialize
void chip8_init(Chip8 *chip8);

// Emulate Cycle
void chip8_Cycle(Chip8 *chip8);

bool chip8_load_game(Chip8 *chip8, const char *filename);

#endif // chip8.h
