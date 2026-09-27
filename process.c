#include "process.h"


PROCESS* PROCESS_new(RAM* ram,struct Display* display,struct Keyboard* keyboard,struct Speaker* speaker){
	PROCESS* process = (PROCESS*)malloc(sizeof(PROCESS));
	if(process){
		process->registres = calloc(0,sizeof(uint8_t)*17);
		process->stack = calloc(0,sizeof(uint16_t)*17);
		
		process->I = 0;
		process->DT = 0;
		process->ST = 0;
		process->PC = 0x200;
		process->SP = 0;
		
		process->ram = ram;
		process->display = display;
		process->keyboard = keyboard;
		process->speaker = speaker;
	}
	return process;
}

void PROCESS_delete(PROCESS* process){
	if(process){
		Speaker_destroy(process->speaker);
		Keyboard_destroy(process->keyboard);
		Display_destroy(process->display);
		RAM_delete(process->ram);
		free(process->registres);
		free(process->stack);
		free(process);
	}
}

int CLS_00E0(PROCESS* process){
	return Display_CLS(process->display);
}

int RET_00EE(PROCESS* process){
	if(process){
		process->PC = process->stack[process->SP-1];
		process->SP -= 1;
		return 0;
	} else {
		return 1;
	}
}

int JP_1nnn(PROCESS* process, uint16_t nnn){
	if(process){
		process->PC = nnn;
		return 0;
	} else {
		return 1;
	}
}

int CALL_2nnn(PROCESS* process, uint16_t nnn){
	if(process){
		process->stack[process->SP ] = process->PC;
		process->SP += 1;
		process->PC = nnn;
		return 0;
	} else {
		return 1;
	}
}

int SE_3xkk(PROCESS* process, uint8_t x, uint8_t kk){
	if(process){
		uint8_t mask = 0x0f;
		if(process->registres[mask & x] == kk){
			process->PC += 2;
		}
		return 0;
	} else {
		return 1;
	}
}

int SNE_4xkk(PROCESS* process, uint8_t x, uint8_t kk){
	if(process){
		uint8_t mask = 0x0f;
		if(process->registres[mask & x] != kk){
			process->PC += 2;
		}
		return 0;
	} else {
		return 1;
	}
}

int SE_5xy0(PROCESS* process, uint8_t x, uint8_t y){
    if(process){
        uint8_t mask = 0x0f;
        if(process->registres[mask & x] == process->registres[mask & y]){
            process->PC += 2;
        }
        return 0;
    } else {
        return 1;
    }
}

int LD_6xkk(PROCESS* process, uint8_t x, uint8_t kk){
    if(process){
        uint8_t mask = 0x0f;
        process->registres[mask & x] = kk;
        return 0;
    } else {
        return 1;
    }
}

int ADD_7xkk(PROCESS* process, uint8_t x, uint8_t kk) {
    if (process) {
        uint8_t mask = 0x0f;
        process->registres[mask & x] = process->registres[mask & x] + kk;
        return 0;
    } else {
        return 1;
    }
}

int LD_8xy0(PROCESS* process, uint8_t x, uint8_t y){
    if (process) {
        uint8_t mask = 0x0f;
        process->registres[mask & x] = process->registres[mask & y];
        return 0;
    } else {
        return 1;
    }
}

int OR_8xy1(PROCESS* process, uint8_t x, uint8_t y){
    if (process) {
        uint8_t mask = 0x0f;
		uint8_t tmp = process->registres[mask & x] | process->registres[mask & y];
        process->registres[mask & x] = tmp;
		process->registres[0xf] = 0;
        return 0;
    } else {
        return 1;
    }
}

int AND_8xy2(PROCESS* process, uint8_t x, uint8_t y){
    if (process) {
        uint8_t mask = 0x0f;
		uint8_t tmp = process->registres[mask & x] & process->registres[mask & y];
        process->registres[mask & x] = tmp;
        process->registres[0xf] = 0;
		return 0;
    } else {
        return 1;
    }
}

int XOR_8xy3(PROCESS* process, uint8_t x, uint8_t y){
    if (process) {
        uint8_t mask = 0x0f;
		uint8_t tmp = process->registres[mask & x] ^ process->registres[mask & y];
        process->registres[mask & x] = tmp;
		process->registres[0xf] = 0;
        return 0;
    } else {
        return 1;
    }
}

int ADD_8xy4(PROCESS* process, uint8_t x, uint8_t y){
    if (process) {
        uint8_t mask = 0x0f;
        uint16_t tmp = (uint16_t)process->registres[mask & x] + process->registres[mask & y];
        process->registres[mask & x] = tmp&0xff;
        if (tmp > 0xff) {
            process->registres[0xf] = 1;
        } else {
            process->registres[0xf] = 0;
        }

        return 0;
    } else {
        return 1;
    }
}

int SUB_8xy5(PROCESS* process, uint8_t x, uint8_t y){
    if (process) {
        uint8_t mask = 0x0f;
		uint8_t tmp  = process->registres[mask & x];
		process->registres[mask & x] = tmp - process->registres[mask & y];
        if (tmp >= process->registres[mask & y]){
            process->registres[0xf] = 1;
        } else {
            process->registres[0xf] = 0;
        }
        return 0;
    } else {
        return 1;
    }
}

int SHR_8xy6(PROCESS* process, uint8_t x, uint8_t y){
    uint8_t mask = 0x0f;
    if (process && (process->registres[mask & x] != 0)) {
        uint8_t tmp = process->registres[mask & y];
        process->registres[mask & x] = tmp/2;
		if (tmp & 1){
            process->registres[0xf] = 1;
        } else {
            process->registres[0xf] = 0;
        }
        return 0;
    } else {
        return 1;
    }
}

int SUBN_8xy7(PROCESS* process, uint8_t x, uint8_t y){
    if (process) {
        uint8_t mask = 0x0f;
		uint8_t tmp = process->registres[mask & x];
        process->registres[mask & x] = process->registres[mask & y] - process->registres[mask & x];
        if (tmp <= process->registres[mask & y]){
            process->registres[0xf] = 1;
        } else {
            process->registres[0xf] = 0;
        }
        return 0;
    } else {
        return 1;
    }
}

int SHL_8xyE(PROCESS* process, uint8_t x, uint8_t y){
    if (process) {
		uint8_t mask = 0x0f;
		uint8_t tmp = process->registres[mask & y];
        
		process->registres[mask & x] = tmp*2;
		
		tmp = tmp&0x80;
        if (tmp == 0x80){
            process->registres[0xf] = 1;
        } else {
            process->registres[0xf] = 0;
        }
        return 0;
    } else {
        return 1;
    }
}

int SNE_9xy0(PROCESS* process, uint8_t x, uint8_t y){
    if (process) {
        uint8_t mask = 0x0f;
        if (process->registres[mask & x] != process->registres[mask & y]) {
            process->PC += 2;
        }
        return 0;
    } else {
        return 1;
    }
}

int LD_Annn(PROCESS* process, uint16_t nnn){
    if (process) {
        process->I =  nnn;
        return 0;
    } else {
        return 1;
    }
}

int JP_Bnnn(PROCESS* process, uint16_t nnn){
    if (process) {
        process->PC =  nnn + process->registres[0];
        return 0;
    } else {
        return 1;
    }
}

int RND_Cxkk(PROCESS* process, uint8_t x, uint8_t kk){
    if (process) {
        srand(time(NULL));
        uint8_t r = rand()%256;
        uint8_t mask = 0x0f;
        process->registres[mask & x] = kk & r;
        return 0;
    } else {
        return 1;
    }
}

int DRW_Dxyn(PROCESS* process, uint8_t x, uint8_t y, uint8_t n){
	if (process) {
		uint8_t mask = 0x0f;
		struct Sprite* sprite = (struct Sprite*)malloc(sizeof(struct Sprite));
		
		Sprite_init(sprite,n);
		for(int i=0; i<(n&mask); ++i){
			Sprite_add(sprite,RAM_read(process->ram, process->I+i));
		}
		Display_DRW(process->display,sprite,process->registres[x],process->registres[y],&process->registres[0xf]);
		
        return 0;
    } else {
        return 1;
    }
}

int SKP_Ex9E(PROCESS* process, uint8_t x){
	if(process){
		uint8_t mask = 0x0f;
		int tmp = 0;
		Keyboard_get(process->keyboard,process->registres[mask & x],&tmp);
		if (tmp == KEY_DOWN){
			process->PC += 2;
		}
		return 0;
	} else {
		return 1;
	}
}

int SKNP_ExA1(PROCESS* process, uint8_t x){
	if(process){
		uint8_t mask = 0x0f;
		int tmp = 0;
		Keyboard_get(process->keyboard,process->registres[mask & x],&tmp);
		if (tmp == KEY_UP){
			process->PC += 2;
		}
		return 0;
	} else {
		return 1;
	}
}

int LD_Fx07(PROCESS* process, uint8_t x){
    if (process) {
        uint8_t mask = 0x0f;
        process->registres[mask & x] = process->DT;
        return 0;
    } else {
        return 1;
    }
}

int LD_Fx0A(PROCESS* process, uint8_t x){
	if(process){
		uint8_t mask = 0x0f;
		Keyboard_wait(process->keyboard,&process->registres[mask & x]);
		return 0;
	} else {
		return 1;
	}
}

int LD_Fx15(PROCESS* process, uint8_t x){
    if (process) {
        uint8_t mask = 0x0f;
        process->DT = process->registres[mask & x];
        return 0;
    } else {
        return 1;
    }
}

int LD_Fx18(PROCESS* process, uint8_t x){
	if (process) {
        uint8_t mask = 0x0f;
        process->ST = process->registres[mask & x];
        return 0;
    } else {
        return 1;
    }
}

int ADD_Fx1E(PROCESS* process, uint8_t x){
    if (process) {
        uint8_t mask = 0x0f;
        process->I = process->I + process->registres[mask & x];
        return 0;
    } else {
        return 1;
    }
}

int LD_Fx29(PROCESS* process, uint8_t x){
	if (process) {
		uint8_t mask = 0x0f;
		process->I = process->registres[mask & x];
		return 0;
	} else {
		return 1;
	}
}

int LD_Fx33(PROCESS* process, uint8_t x){
    if (process) {
        uint8_t mask = 0x0f;
        uint8_t tmp = process->registres[mask & x];

        process->ram->ram[process->I+2] = tmp % 10;
        tmp = tmp/10;

        process->ram->ram[process->I+1] = tmp % 10;
        tmp = tmp/10;

        process->ram->ram[process->I] = tmp % 10;

        return 0;
    } else {
        return 1;
    }
}

int LD_Fx55(PROCESS* process, uint8_t x){
    if (process) {
        uint8_t mask = 0x0f;
        for (uint8_t indice=0;indice<=(mask & x);++indice){
			RAM_write(process->ram,process->I,process->registres[indice]);
			process->I += 1;
        }
        return 0;
    } else {
        return 1;
    }
}

int LD_Fx65(PROCESS* process, uint8_t x){
    if (process) {
        uint8_t mask = 0x0f;
        for (uint8_t indice=0;indice<=(mask & x);++indice){
			process->registres[indice] = RAM_read(process->ram,process->I);
			process->I += 1;
        }
        return 0;
    } else {
        return 1;
    }
}

uint8_t valeur_x(uint16_t instruction){
	uint16_t mask = 0x0f00;
	uint16_t result = instruction&mask;
	result = result>>8;
	
	return result;
}
uint8_t valeur_y(uint16_t instruction){
	uint16_t mask = 0x00f0;
	uint16_t result = instruction&mask;
	result = result>>4;
	
	return result;
}
uint8_t valeur_n(uint16_t instruction){
	uint16_t mask = 0x000f;
	uint16_t result = instruction&mask;
	
	return result;
}
uint16_t valeur_nnn(uint16_t instruction){
	uint16_t mask = 0x0fff;
	uint16_t result = instruction&mask;
	
	return result;
}
uint8_t valeur_kk(uint16_t instruction){
	uint16_t mask = 0x00ff;
	uint16_t result = instruction&mask;
	
	return result;
}

int fetch_decode_execute(PROCESS* process){ // uint16_t concatène 2 octes pour une instruction
	if (process) {
		uint8_t partie1 = RAM_read(process->ram, process->PC);
		uint8_t partie2 = RAM_read(process->ram, process->PC + 1);
		process->PC += 2;

		uint16_t instruction = partie1;
		instruction = instruction<<8;
		instruction = instruction + partie2;
		
		uint16_t mask = 0xf000;
		
		uint16_t valeur_1 = instruction&mask;
		valeur_1 = valeur_1>>12;
		
		if (valeur_1 == 0x0){
			if (instruction == 0x00e0) {
				CLS_00E0(process);
			} else if (instruction == 0x00ee) {
				RET_00EE(process);
			}
		} else if (valeur_1 == 0x1){
			JP_1nnn(process,valeur_nnn(instruction));
		} else if (valeur_1 == 0x2){
			CALL_2nnn(process,valeur_nnn(instruction));
		} else if (valeur_1 == 0x3){
			SE_3xkk(process,valeur_x(instruction),valeur_kk(instruction));
		} else if (valeur_1 == 0x4){
			SNE_4xkk(process,valeur_x(instruction),valeur_kk(instruction));
		} else if (valeur_1 == 0x5){
			SE_5xy0(process,valeur_x(instruction),valeur_y(instruction));
		} else if (valeur_1 == 0x6){
			LD_6xkk(process,valeur_x(instruction),valeur_kk(instruction));
		} else if (valeur_1 == 0x7){
			ADD_7xkk(process,valeur_x(instruction),valeur_kk(instruction));
		} else if (valeur_1 == 0x8){
			valeur_1 = instruction&0x000f;
			if (valeur_1 == 0x0){
				LD_8xy0(process,valeur_x(instruction),valeur_y(instruction));
			} else if (valeur_1 == 0x1){
				OR_8xy1(process,valeur_x(instruction),valeur_y(instruction));
			} else if (valeur_1 == 0x2){
				AND_8xy2(process,valeur_x(instruction),valeur_y(instruction));
			} else if (valeur_1 == 0x3){
				XOR_8xy3(process,valeur_x(instruction),valeur_y(instruction));
			} else if (valeur_1 == 0x4){
				ADD_8xy4(process,valeur_x(instruction),valeur_y(instruction));
			} else if (valeur_1 == 0x5){
				SUB_8xy5(process,valeur_x(instruction),valeur_y(instruction));
			} else if (valeur_1 == 0x6){
				SHR_8xy6(process,valeur_x(instruction),valeur_y(instruction));
			} else if (valeur_1 == 0x7){
				SUBN_8xy7(process,valeur_x(instruction),valeur_y(instruction));
			} else if (valeur_1 == 0xe){
				SHL_8xyE(process,valeur_x(instruction),valeur_y(instruction));
			}
		} else if (valeur_1 == 0x9){
			SNE_9xy0(process,valeur_x(instruction),valeur_y(instruction));
		} else if (valeur_1 == 0xa){
			LD_Annn(process,valeur_nnn(instruction));
		} else if (valeur_1 == 0xb){
			JP_Bnnn(process,valeur_nnn(instruction));
		} else if (valeur_1 == 0xc){
			RND_Cxkk(process,valeur_x(instruction),valeur_kk(instruction));
		} else if (valeur_1 == 0xd){
			DRW_Dxyn(process,valeur_x(instruction),valeur_y(instruction),valeur_n(instruction));
		} else if (valeur_1 == 0xe){
			valeur_1 = instruction&0x00ff;
			
			if (valeur_1 == 0x9E){
				SKP_Ex9E(process,valeur_x(instruction));
			} else if (valeur_1 == 0xA1){
				SKNP_ExA1 (process,valeur_x(instruction));
			}
		} else if (valeur_1 == 0xf){
			valeur_1 = instruction&0x00f0;
			valeur_1 = valeur_1 >> 4;
			
			if (valeur_1 == 0x0){
				valeur_1 = instruction&0x000f;
				if (valeur_1 == 0x7){
					LD_Fx07(process,valeur_x(instruction));
				} else if (valeur_1 == 0xa){
					LD_Fx0A(process,valeur_x(instruction));
				}
			} else if (valeur_1 == 0x1){
				valeur_1 = instruction&0x000f;
				if (valeur_1 == 0x5){
					LD_Fx15(process,valeur_x(instruction));
				} else if (valeur_1 == 0x8) {
					LD_Fx18(process,valeur_x(instruction));
				} else if (valeur_1 == 0xe){
					ADD_Fx1E(process,valeur_x(instruction));
				}
			} else if (valeur_1 == 0x2){
				LD_Fx29(process,valeur_x(instruction));
			} else if (valeur_1 == 0x3){
				LD_Fx33(process,valeur_x(instruction));
			} else if (valeur_1 == 0x5){
				LD_Fx55(process,valeur_x(instruction));
			} else if (valeur_1 == 0x6){
				LD_Fx65(process,valeur_x(instruction));
			}
		}
        return 0;
    } else {
        return 1;
    }
}