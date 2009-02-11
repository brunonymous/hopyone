/**
 * @file general.h 
 * @brief Main function is where the program starts execution 
 * @created 2007-11-12 
 * @date 2009-02-11 
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
#ifndef _general_h
#define _general_h

/**************************************************
                STRUCTS DECLARATIONS
**************************************************/

typedef struct
{
    /* Mouse coordinates */
    int motionX, motionY;
    /* Old Mouse coordinates */
    int oldMotionX, oldMotionY;
    /* Mouse coordinates when clicking */
    int clicX, clicY;
    /* Mouse buttons' states */
    int leftClic, rightClic;
}StructMouse;

/**************************************************
                    VARIABLES
**************************************************/

extern StructMouse mouse;

/** Permits you to switch fullscrenn on/oof */
extern bool fullscreen;

/* Timer */
extern int NewTick, LastTick;

/* Number of frames per seconde */
extern int fps;

/* Screen buffer */
extern SDL_Surface *screen;
extern SDL_Surface *oldscreen;

/* Events' list */
extern int event[100];

/* Games' texts in the chosen language */
extern char text[40][255];


/**************************************************
                FUNCTIONS DECLARATIONS
**************************************************/

/** This function permits you to initialize SDL
* \param windowName : The name of the window
* \param iconame : The destination of the icon image
*/
SDL_Surface * initSDL(char *windowName, char *iconame);

/** This function permits you to load a picture in "any" format thanks to SDL_image
* The by default SDL_image dll with the game supports only bmp and png.
* But you can use another SDL_image dll, with more formats, and consequently more disk space used...
* \param filename : The location of the picture
*/
SDL_Surface *loadPicture(char *filename);

/** This function permits you to display a full square in a surface
* \param surface : The surface you want to draw
* \param squarex : The abscissa of the square
* \param squarey : The ordonnee of the square
* \param squarew : The width of the square
* \param squareh : The height of the square
* \param r : The red variant
* \param g : The green variant
* \param b : The blue variant
*/
void displayFullSquare(SDL_Surface *surface, int rectx, int recty, int rectw, int recth, int r, int v, int b);

/** This function permits you to display an empty square in a surface
* \param surface : The surface you want to draw
* \param squarex : The abscissa of the square
* \param squarey : The ordonnee of the square
* \param squarew : The width of the square
* \param squareh : The height of the square
* \param r : The red variant
* \param g : The green variant
* \param b : The blue variant
*/
void displayEmptySquare(SDL_Surface *surface, int rectx, int recty, int rectw, int recth, int r, int v, int b);

/** This function permits you to copy a surface area in another surface at the wished coordinates 
* \param srcS : The surface you want to copy
* \param destS : The surface where you want to copy
* \param srcx : The abscissa of the area
* \param srcy : The ordonnee of the area
* \param srcw : The width of the area
* \param srch : The height of the area
* \param srcx : The abscissa of the new area's position
* \param srcy : The ordonnee of the new area's position
*/
void copyPicture(SDL_Surface *srcS, SDL_Surface *destS, int srcx, int srcy, int srcw, int srch, int destx, int desty);

/** This function totally clear a surface.
* \param surface : the surface you want to clear
*/
void clearSurface(SDL_Surface *surface);

/** This function manages the keys events and the mouse events
* Following the key pressed or the mouse button clicked, the event array is updated
*/
int manageKeys();

/** This function permits to wait for a key press. 
* If a key is pressed, returns 1, else returns 0.
*/
int keyPressed();

/*! Displays a line between two points 
* \param surface : the surface which will be used
* \param x1 : the abscissa of the first point
* \param y1 : the ordonnee of the first point
* \param x2 : the abscissa of the second point
* \param y2 : the ordonnee of the second point
*/
void displayLine(SDL_Surface *surface, int x1, int y1, int x2, int y2, int r, int g, int b);

/** This function permits you to get the color of a chosen pixel in a surface
* \param surface : the surface you want to get infos
* \param x : the abscissa of the pixel you want to get infos
* \param y : the ordonnee of the pixel you want to get infos
*/
Uint32 getpixel(SDL_Surface *surface, int x, int y);

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

SDL_Surface * displayMask(SDL_Surface *sprite);

#endif
