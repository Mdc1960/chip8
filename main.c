#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "emulator.h"

// Fonction pour mettre à jour les timers en fonction du temps écoulé
void updateTimers(EMULATOR *emulator) {
    clock_t currentTime = clock();
    double time_passed = (double)(currentTime - emulator->lastUpdateTime) / 250;

    if (emulator->process->DT > 0) {
        emulator->process->DT -= (uint8_t)time_passed;
        if (emulator->process->DT < 0) {
            emulator->process->DT = 0;
        }
    }

    if (emulator->process->ST > 0) {
        emulator->process->ST -= (uint8_t)time_passed;
        if (emulator->process->ST < 0) {
            emulator->process->ST = 0;
        }
    }
	
	if(emulator->process->ST > 0){
		Speaker_on(emulator->speaker);
	} else {
		Speaker_off(emulator->speaker);
	}
	
    emulator->lastUpdateTime = currentTime; // Met à jour le temps de la dernière mise à jour
}

int main(int argc, char *argv[]) {
	EMULATOR* emul = EMULATOR_new();
	int x = LD_ram(emul->process);
	unsigned int count = 0;

	if(x == 0){
		while(1){
			fetch_decode_execute(emul->process);
			count++;
			
			if((count%8)==0){
				updateTimers(emul);
				Display_update(emul->display);
			}
			
			SDL_Delay(2);
		}
		EMULATOR_delete(emul);
	}
	return 0;
}