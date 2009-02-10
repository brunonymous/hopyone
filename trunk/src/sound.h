#ifndef _exemple_h
#define _exemple_h

/**************************************************
                    INCLUDES
**************************************************/

/* none */

/**************************************************
                    DEFINES
**************************************************/

/* none */

/**************************************************
                STRUCTS DECLARATIONS
**************************************************/

/* none */

/**************************************************
                    VARIABLES
**************************************************/

extern int soundvol, musicvol;
extern Mix_Music *mod;
extern Mix_Chunk *samples[20];

/**************************************************
                FUNCTIONS DECLARATIONS
**************************************************/

void initSound();

void stopSound();

void loadMusic(char *pak, char *name);

void openSound(char *pak, char *name, int n);

void playSound(int n);

void cleanSound();


#endif
