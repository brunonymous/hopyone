#ifndef __TLK_COMPTEUR_H__
#define __TLK_COMPTEUR_H__

typedef struct {

	unsigned int	count;			/* nombre de fois que le jeu a été lancé depuis l'installation */

	char			name[16];		/* nom du jeu */

} TLKGameCount;


int TLKUpdateGameCount(char *gamename);
void TLKFreeGameCount();


#endif 
