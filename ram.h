#ifndef RAM_HEADER_GUARD
#define RAM_HEADER_GUARD

/**
 * \file  ram.h
 * \brief Contient les dÃ©claration pour la structure de donnÃ©es \ref RAM ,ainsi que les fonctions associÃ©es
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * \struct RAM
 * \brief  Structure de la reprÃ©sentation d'une RAM.
 *
 * utiliser les fonctions en dessous
 */
typedef struct RAM
{
	/**
	* \brief La ram de notre structure RAM.
	*
	* \p ram a une taille de 4096 uint8_t.
	*/
    uint8_t* ram;
}RAM;

/**
 * \relates RAM
 * \brief   Alloue une RAM vide.
 * \return  La nouvelle RAM sur succÃ¨s, NULL si il y a une erreur.
 *
 * La RAM pourra Ãªtre passÃ© Ã  la fonction \ref RAM_delete pour dÃ©sallouer la RAM.
 */
RAM* RAM_new();

/**
 * \relates RAM
 * \brief   Desalloue la memoire d'une RAM.
 * \param   ram La RAM Ã  desallouer.
 */
void RAM_delete(RAM* ram);

/**
 * \relates RAM
 * \brief   Ecrit une valeur dans la memoire d'une RAM.
 * \param   ram La RAM Ã  ecrire.
 * \param   adr L'adresse oÃ¹ il faut ecrire.
 * \param   val La valeur Ã  ecrire.
 */
int RAM_write(RAM* ram, uint16_t adr, uint8_t val);

/**
 * \relates RAM
 * \brief   Affiche la valeur a l'adresse dans la memoire d'une RAM.
 * \param   ram La RAM a rechercher.
 * \param   adr L'adresse de la valeur qu'il faut afficher.
 */
uint8_t RAM_read(RAM* ram, uint16_t adr);

// end of ifndef LIST_HEADER_GUARD
#endif
