/**
 * @file main.c
 * @brief Main function is where the program starts execution 
 * @created 2007-11-12 
 * @date 2009-02-10 
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
#include "global.h"

//#define SHAREWARE

//Surfaces
SDL_Surface *IMG_map = NULL;
SDL_Surface *IMG_spoing = NULL;
SDL_Surface *IMG_cable = NULL;
SDL_Surface *IMG_fire = NULL;
SDL_Surface *IMG_body = NULL;
SDL_Surface *IMG_mouse = NULL;
SDL_Surface *gameover = NULL;
SDL_Surface *fond_menu = NULL;

//Decor
SDL_Surface *IMG_decor = NULL, *IMG_old_decor = NULL;

Sint32 actu_score = 2, actu_ammo = 2, actu_time = 2, actu_life = 2;

/**
 * The main function is where the program starts execution
 */
Sint32
//main (Sint32 argn, char *argc[])
main ()
{

  /* Sint32 CountLaunch = TLKUpdateGameCount("HopyOne"); */
  //TLKFreeGameCount();

  ReadConfigFile2 ();

  if (!init_game ())
    {
      return 0;
    }
  ReadConfigFile ();
  initFonts ();
  loadText ();
  resetGame ();
/*
    #ifdef SHAREWARE
       displayPage(1,CountLaunch);
    #endif
*/
  initMap ();
  manageMenu ();
/*
    #ifdef SHAREWARE
       displayPage(2,CountLaunch);
    #endif
*/
  freeGame ();
  return 0;
}
