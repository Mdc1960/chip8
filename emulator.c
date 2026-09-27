//
// Created by aumon on 11/01/2024.
//

#include "emulator.h"

EMULATOR* EMULATOR_new(){
	EMULATOR* emulateur = (EMULATOR*)malloc(sizeof(EMULATOR));
	if(emulateur){
		struct Display* display = (struct Display*)malloc(sizeof(struct Display));
		struct Keyboard* keyboard = (struct Keyboard*)malloc(sizeof(struct Keyboard));
		struct Speaker* speaker = (struct Speaker*)malloc(sizeof(struct Speaker));
		
		Display_init(display,10);
		Keyboard_init(keyboard);
		Speaker_init(speaker);
		
		emulateur->lastUpdateTime = clock();
		
		emulateur->display = display;
		emulateur->keyboard = keyboard;
		emulateur->speaker = speaker;
		
		emulateur->ram = RAM_new();
		
		emulateur->process = PROCESS_new(emulateur->ram,emulateur->display,emulateur->keyboard,emulateur->speaker);
	}
	return emulateur;
}

int EMULATOR_delete(EMULATOR* emulateur){
	if(emulateur){
		Display_destroy(emulateur->display);
		Keyboard_destroy(emulateur->keyboard);
		Speaker_destroy(emulateur->speaker);
		RAM_delete(emulateur->ram);
		PROCESS_delete(emulateur->process);
		
		free(emulateur);
		return 0;
	} else {
		return 1;
	}
}

int LD_ram(PROCESS* process){
	FILE *ptr;
	
	ptr = fopen("/mnt/hgfs/code/Developpement/C/prog_imperative/chip8/code/5-quirks.ch8","rb");  // r for read, b for binary

	if (!ptr) {
		printf("erreur load ram\n");
		return 1;
	}
	
	uint8_t byte;
	int i = 0x200;
	while (fread(&byte, sizeof(uint8_t), 1, ptr) == 1 && i<4096) {
		RAM_write(process->ram, i, byte);
		i+=1;
	}
	fclose(ptr);
	return 0;
}