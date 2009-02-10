#ifndef _burst_h
#define _burst_h

/**************************************************
                    INCLUDES
**************************************************/


/**************************************************
                    DEFINES
**************************************************/

#define LIFE_STAR 32
#define LIFE_BURST 32
#define LIFE_BIGBURST 64
#define STARS 100

#define LIFE_COLLAPSINGBLOCK 30

/**************************************************
                STRUCTS DECLARATIONS
**************************************************/


/**************************************************
                    VARIABLES
**************************************************/


/**************************************************
                FUNCTIONS DECLARATIONS
**************************************************/

void manageBursts();

void displayBursts();

void addBurst(int x, int y);

void addBigBurst(int x, int y);

void initStar();

void manageStars();

void addStar(int x, int y);

void displayStar();

void addStars();

void displayCollapsingBlocks();

void addCollapsingBlock(int x, int y);

void loadCollapsingBlocks();

void collapseCollapsingBlock(int x, int y);

void manageCollapsingBlocks();

#endif
