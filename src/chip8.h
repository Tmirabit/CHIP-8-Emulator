// chip8.h
#ifndef CHIP8_H
#define CHIP8_H

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
} Chip8;

// Initialize
void chip8_init(Chip8 *chip8);

// Emulate Cycle
void chip8_Cylcle(Chip8 *chip8);

#endif // chip8.h
