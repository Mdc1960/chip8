//
// Created by aumon on 11/01/2024.
//

#ifndef PARALLEL_EMULATOR_H
#define PARALLEL_EMULATOR_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "process.h"

/**
 * \struct EMULATOR
 * \brief  Structure de la reprÃ©sentation d'un processus.
 *
 * utiliser les fonctions en dessous
 */
typedef struct EMULATOR{
	/**
	* \brief Le pointeur sur notre processeur de notre structure EMULATOR.
	*
	* \p process Le pointeur sur notre processeur PROCESS de notre EMULATOR.
	*/
	PROCESS* process;
	/**
	* \brief Le pointeur sur notre ram de notre structure EMULATOR.
	*
	* \p process Le pointeur sur notre ram RAM de notre EMULATOR.
	*/
	RAM* ram;
	/**
	* \brief Le pointeur sur notre display de notre structure EMULATOR.
	*
	* \p process Le pointeur sur notre display Display de notre EMULATOR.
	*/
	struct Display* display;
	/**
	* \brief Le pointeur sur notre keyboard de notre structure EMULATOR.
	*
	* \p process Le pointeur sur notre keyboard Keyboard de notre EMULATOR.
	*/
	struct Keyboard* keyboard;
	/**
	* \brief Le pointeur sur notre speaker de notre structure EMULATOR.
	*
	* \p process Le pointeur sur notre speaker Speaker de notre EMULATOR.
	*/
	struct Speaker* speaker;
	/**
	* \brief La date de la dernière instruction du processeur de notre structure EMULATOR.
	*
	* \p process La variable sur la date de la dernière instruction du processeur de type clock_t de notre EMULATOR.
	*/
	clock_t lastUpdateTime;
}EMULATOR;

/**
 * @brief Alloue un EMULATOR vide.
 * @return new EMULATOR on succes, NULL if error. 
 */
EMULATOR* EMULATOR_new();

/**
 * @brief Desalloue la memoire d'un EMULATOR.
 * @return 0 on success, 1 in case of an error
 */
int EMULATOR_delete();

/**
 * @brief charge la ram
 * @param process
 * @return 0 on success, 1 in case of an error
 */
int LD_ram(PROCESS* process);

#endif //PARALLEL_EMULATOR_H
