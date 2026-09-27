#include "ram.h"

RAM* RAM_new(){
	RAM* ram = (RAM*)malloc(sizeof(RAM));
	if(ram){
		ram->ram = (uint8_t*)malloc(4096*sizeof(uint8_t));
	}
	return ram;
}

void RAM_delete(RAM* ram){
	if(ram){
		free(ram->ram);
		free(ram);
	}
}

int RAM_write(RAM* ram, uint16_t adr, uint8_t val){
	if(adr <= 4095){
		ram->ram[adr] = val;
		return 0;
	} else {
		return 1;
	}
}

uint8_t RAM_read(RAM* ram, uint16_t adr){
	if(adr <= 4095){
		return ram->ram[adr];
	} else {
		return 1;
	}
}