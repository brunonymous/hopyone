/**
 * @file hopy.h 
 * @brief 
 * @created 2007-11-12 
 * @date 2007-11-12
 */
/*
 * copyright (c) 1992-2009 TLK Games all rights reserved
 * $Id$
 *
 * Hopy One is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Hopy One is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */
#ifndef _hopy_h
#define _hopy_h

/**************************************************
                    DEFINES
**************************************************/

/********** COLLISIONS CONVERSIONS **********/
/* #define X2(a)       ((a)*2) */
/* #define X2(a)       (((a)*X)/640) */
#define X2(a)       (int)((a)*1.9)
#define Y2(a)       ((a)*2)
/* #define Y2(a)       (((a)*Y)/480) */

/********* Collisions' definitions ************/

#define SPOINGFORM 0
#define SPHEREFORM 1

#define COL_VIDE        0  
#define SPOING          1  
#define COL_SPECIAL_TAB 2       
#define COL_SECRET      4  
                           
#define COL_TIR_D       20 
#define ZONE_TIR        21

#define COL_BONUS       98 
#define COL_NORMAL      99 
#define COL_EFFRITE     101
#define COL_NO_CABLE	103	         
#define COL_MORTEL		109	          
                           
#define COL_PORTE_0     224
#define COL_PORTE_1     225
#define COL_PORTE_2     226
#define COL_PORTE_3     227
#define COL_PORTE_4     228
#define COL_PORTE_5     229
#define COL_PORTE_6     230
#define COL_PORTE_7     231
#define COL_PORTE_8     232
#define COL_PORTE_9     233
#define COL_TRANS_SB    234
#define COL_TRANS_BS    235

#define COL_ENNEMY      250

/* Games' definitions */

#define NBSHOOTS  20
#define NBSHOOTSE 20
#define NBENNEMYS 10
#define NBDOORS   10

/*

#define FX      0.95 
#define FY      0.95 
#define AY 1000

#define GRAVITY 0.5
#define SHOCK   0.55
#define LEVEL   0
#define SCORE   0
#define AX 20 
#define FIRESPEED 150

*/

#define CHEAT

#ifdef CHEAT
   #define UNDYING 200
   #define AMMO    100
   #define LIFE    3
#else
   #define UNDYING 200
   #define AMMO    100
   #define LIFE    3
#endif
   
/*#define SPOINGW    X2(39)
#define SPOINGH    Y2(53)
#define SPOINGHMIN Y2(42)
#define SPHEREW    X2(27)
#define SPHEREH    Y2(32)

#define FOOTH   Y2(34)
#define HEADH   Y2(32)
#define CANNONH  Y2(18)

#define FireW    17
#define FireH     5
*/
#define SPOINGW    38
#define SPOINGH    56
#define SPOINGHMIN 46
#define SPHEREW    25 
#define SPHEREH    30

#define FOOTH   34
#define HEADH   30
#define CANNONH  25

#define FireW    17
#define FireH     5

#define NB_BURSTS    20
#define NB_BIGBURSTS 20
#define NB_STARS     20
#define NB_COLLAPSINGBLOCK 100

/**************************************************
                STRUCTS DECLARATIONS
**************************************************/
typedef struct 
{
   int x, y;
}StructCoord;

typedef struct 
{
    /* Coordinates and dimensions of the ennemy */
    int x, y, w, h;
    /* Shoots' information */
    int cadence_tir, type_tir;
    /* Ennemy's life */
    int resistance;
    /* ennemy's kind */
    int kind;
    /* bonus you can earn when beating this ennemy */
    int bonus;
    /* direction (horizontal/vertical) and sens (negative, positive) of the ennemy */
    int direction, sens;
    int mask;
}StructEnnemy;

typedef struct 
{
    int x, y, cad;
    int dx, dy, debx, deby, finx, finy;
    int speed;
}StructDetectingShoot;

typedef struct 
{
    int x, y;
    float dx, dy;
    int state;
    int power;
    int type;
    int speed;
}StructShoot;

typedef struct
{
    float x;
    float y;
    int w;
    int h;
    float dx;
    float dy;
    float ax;
    float ay;
    float speed;
    /** Contains the number of life you have */
    int life;
    /** Contains your score */
    int score;
    /** Indicates how ammunitions you have */
    int ammo;
    /** Indicates which level you currently run */
    int level;
    /** Makes you undying during some time */
    int undying;
} StructGuy;

typedef struct 
{
   int x, y;
   int life;
}StructBurst;

/**************************************************
                    VARIABLES
**************************************************/

/** Map datas' array */
extern int Map[];
/** Collisions datas' array */
extern int Col[];

/** Doors' array
* 1st value of the second column : the state of the door (existing or not)
* 2nd value of the second column : the game board's destination of the door 
*/
extern int Door[NBDOORS][2];

/** Spoing's array
* Those arrays contain the graphic coordinates of the spoing, point per point 
*/
extern int arrayXSpoing[], arrayYSpoing[];

/** Bonus' array */
extern int Bonus[100];
extern int NbBonus;

extern int CableLaunched;

extern float XLeftCable, YLeftCable, XRightCable, YRightCable, XCable, YCable;

extern int agrandissementMax, agrandissement;

extern float saut, sautacc, sautmax;

extern int form;

extern int first;

extern int apparition;

extern int die;

extern int FX;
extern int FY;
extern int GRAVITY;
extern int SHOCK;
extern int LEVEL;
extern int SCORE;  
extern int AX;
extern int AY;
extern int FIRESPEED;



extern SDL_Joystick *joy;

/** Ennemys' array*/
extern StructEnnemy ennemy[NBENNEMYS];

/** Detecting Areas' array*/
extern StructDetectingShoot detectingArea[10];

/** Detecting shoots' array*/
extern StructShoot detectingShoot[NBSHOOTSE];

/** Ennemy shoots' array*/
extern StructShoot ennemyShoot[NBSHOOTSE];

/** Player shoots' array*/
extern StructShoot playerShoot[NBSHOOTS];

/** Player's array*/
extern StructGuy player;

/** Bursts' array*/
extern StructBurst burst[NB_BURSTS], bigburst[NB_BIGBURSTS], stars[NB_STARS];

/** Collapsing Blocks' array*/
extern StructBurst collapsingBlock[NB_COLLAPSINGBLOCK];

/**************************************************
                FUNCTIONS DECLARATIONS
**************************************************/

/** This function clear the game board.
* To know : this function doesn't clear totally the surface, just the game board area.
* \param surface : the surface you want to clear
*/
void clearBoard(SDL_Surface *surface);

/*! This function resets the game datas. */
void resetGame();

/* ! This function return the collision value at the coordinate (x, y). */
/*!
  \param x : the abscissa of the point.
  \param y : the ordonnee of the point.
*/
int getCollision(int x, int y);

/* ! This function put the collision value at the coordinate (x, y). */
/*!
  \param x : the abscissa of the point.
  \param y : the ordonnee of the point.
  \param col : the collision value of the point.
*/
void putCollision(int x, int y, int col);

/*! Frees all the game allocations */
void freeGame();

bool init_game(void);

/*! This function applies the changes due to the death of the player. */
void Die();

/*! Inits the map */
void initMap();

/*! Displays the player, in spoing form */
void displayPlayer();

void displayMap();

void loadCol();

void loadEnnemys();

void loadShoots();

void loadMap();

void loadSpoing();

void initDecor();

void displayCase(SDL_Surface *surface, int i, int j);
void displayShadow(SDL_Surface *surface, int i, int j);

void initFonts();

void openDoors();

/*! This function updates the texts */
void displayInfos();

/*! This function redraw all the texts */
void displayInfos2();

void displayShoots();

void displayEnnemys();

void displayCables();

void animate(SDL_Surface *surface1, SDL_Surface *surface2, int direction);

void refreshZone(char buffer[], int x, int y, int w, int h);

void restart();

int testLeftCollisions();

int testRightCollisions();

int testUpCollisions();

int testUpCollisions2();

int testDownCollisions();

int testGroundCollisions();

int isCollision(int j);
#endif
