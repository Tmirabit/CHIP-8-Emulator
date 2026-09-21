#include <stdio.h>

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
} chip8;

void chip8::initialize()
{
	// Initialize registers and memory once
	pc     = 0x200;
	opcode = 0;
	I      = 0;
	sp     = 0;

	// Clear Display
	for(int i=0; i<sizeof(gfx)/sizeof(gfx); ++i)
		gfx[i] = 0;

	// clear stack
	for(int i=0; i<sizeof(stack)/sizeof(stack); ++i)
		stack[i] = 0;

	// Clear registers v0->VF
	for(int i=0; i<sizeof(V)/sizeof(V); ++i)
		V[i] = 0;

	// Clear memory
	for(int i=0; i<sizeof(memory)/sizeof(memory); ++i)
		memory[i] = 0;
	
	// Load Fontset
	for(int i=0; i<80; ++i)
		memory[i] = chip8_fontset[i];

	// reset timers
	delay_timer = 0;
	sound_timer = 0;
}

void chip::emulateCycle()
{
	// Fetch Opcode
	opcode = memory[pc] << 8 | memory[pc+1]

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
					break:
				case 0x0001: // 0x8XY1
					break:
				case 0x0002: // 0x8XY2
					break:
				case 0x0003: // 0x8XY3
					break:
				case 0x0004: // 0x8XY4
					break:
				case 0x0005: // 0x8XY5
					break:
				case 0x0006: // 0x8XY6
					break:
				case 0x0007: // 0x8XY7
					break:
				case 0x000E: // 0x8XYE
					break:
				default:
					printf("Unknown opcode [0x8000]:  0x%X\n", opcode);
			}
		case 0x9000: // 0x9XY0
			break;
		case 0xA000: // 0xANNN
			I = opcode & 0x0FFF
			pc += 2
			break;
		case 0xB000: // 0xBNNN
			pc = V[0] + (opcode & 0x0FFF)
			break;
		case 0xC000: // 0xCXNN
			break;
		case 0xD000: // 0xDXYN
			break;
		case 0xE000:
			switch(opcode & 0x000F)
				case 0x000E: // 0xEX9E
					break;
				case 0x0001: // 0xEXA1
					break;
				default:
					printf("Unknown opcode [0xE000]:  0x%X\n", opcode);
		case 0xF000:
			switch(opcode & 0x000F)
				case 0x0007: // 0xFX07
					break;
				case 0x000A: // 0xFX0A
					break;
				case 0x0005: // 0xFX15
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
					if(opcode & 0x00F0) == 0x0050:
						// 0xFX55
					else
						// 0xFX65
					break;
				default:					
					printf("Unknown opcode [0xF000]:  0x%X\n", opcode);
}

	//Update timers
	if(delay_timer > 0)
		--delay_timer;

	if(sound_timer > 0)
	{
		if(sound_timer == 1)
			printf("BEEP!\n");
		--sound_timer
	}
}





