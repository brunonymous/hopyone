#ifndef _scores_h
#define _scores_h

/**************************************************
                STRUCTS DECLARATIONS
**************************************************/

typedef struct
{
    Sint32 score;
    char name[100];
}Sscore;

/**************************************************
                    VARIABLES
**************************************************/

extern Sscore HiScore[10];

/**************************************************
                FUNCTIONS DECLARATIONS
**************************************************/

void save(Sint32 nb);

void load(Sint32 nb);

void saveHighScores (void);

void loadHighScores (void);

void resetHighScores (void);

#endif
