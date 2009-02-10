/**
 * @file hopy.c 
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
#include <stdbool.h>
#include "global.h"
Sint32 SHADOW_LEVEL = 200;
/** Map datas' array */
Sint32 Map[line * column * 2];
/** Collisions datas' array */
Sint32 Col[line * column * 2];

/** Doors' array
* 1st value of the second column : the state of the door (existing or not)
* 2nd value of the second column : the game board's destination of the door 
*/
Sint32 Door[10][2];

/** Spoing's array
* Those arrays contain the graphic coordinates of the spoing, point per point 
*/
Sint32 arrayXSpoing[390], arrayYSpoing[390];

/** Bonus' array */
Sint32 Bonus[100];
Sint32 NbBonus;

/** Indicates the state of the cable (launched or not) */
Sint32 CableLaunched;

/*! Some variables... */
Sint32 agrandissementMax = 3, agrandissement = 0;

/** Coordinates of the spoing cable */
float XLeftCable, YLeftCable, XRightCable, YRightCable;
/** Coordinates of the sphere cable */
float XCable, YCable;

/*! Some variables for jumping... */
float saut = 0, sautacc = 10, sautmax = 35;

/*! Indicates the form of the player */
Sint32 form;

/*! Variable for the display 
* We redraw the game's texts each time first>0
*/
Sint32 first;

/*! Indicates if we must display the start player's animation */
Sint32 apparition;

Sint32 die = 0;

Sint32 FX;
Sint32 FY;
Sint32 GRAVITY;
Sint32 SHOCK;
Sint32 LEVEL;
Sint32 SCORE;
Sint32 AX;
Sint32 AY;
Sint32 FIRESPEED;

//Joystick
SDL_Joystick *joy;

/** Ennemys' array*/
StructEnnemy ennemy[NBENNEMYS];

/** Detecting Areas' array*/
StructDetectingShoot detectingArea[10];

/** Ennemy shoots' array*/
StructShoot detectingShoot[NBSHOOTSE];

/** Ennemy shoots' array*/
StructShoot ennemyShoot[NBSHOOTSE];

/** Player shoots' array*/
StructShoot playerShoot[NBSHOOTS];

/** Player's array*/
StructGuy player;

/** Bursts' array*/
StructBurst burst[NB_BURSTS], bigburst[NB_BIGBURSTS], stars[NB_STARS];

/** Collapsing Blocks' array*/
StructBurst collapsingBlock[NB_COLLAPSINGBLOCK];

/**************************************************
                    FUNCTIONS
**************************************************/

/** This function clear the game board.
* To know : this function doesn't clear totally the surface, just the game board area.
* \param surface : the surface you want to clear
*/
void
clearBoard (SDL_Surface * surface)
{
  SDL_Rect rect;

  rect.x = 0;
  rect.y = 0;
  rect.w = X;
  rect.h = line * height;

  SDL_FillRect (surface, &rect, SDL_MapRGB (surface->format, 0, 0, 0));
}

/*! This function resets the game datas. */
void
resetGame (void)
{
  Sint32 i;

  for (i = 0; i < 100; i++)
    Bonus[i] = 0;

  player.life = LIFE;
  player.score = SCORE;
  player.ammo = AMMO;
  player.level = LEVEL;
  player.undying = UNDYING;
  form = SPOINGFORM;

  player.w = SPOINGW;
  player.h = SPOINGH;
}

Sint32
isCollision (Sint32 j)
{
  if (j == 0 || j == COL_SECRET || j == COL_BONUS || j == COL_TRANS_SB
      || j == COL_TRANS_BS || (j >= COL_PORTE_0 && j <= COL_PORTE_9
                               && NbBonus == 0))
    return 0;
  else
    return 1;
}

//! This function return the collision value at the coordinate (x, y).
/*!
  \param x : the abscissa of the point.
  \param y : the ordonnee of the point.
*/
Sint32
getCollision (Sint32 x, Sint32 y)
{
  return Col[y / height * column + x / width];
}

//! This function put the collision value at the coordinate (x, y).
/*!
  \param x : the abscissa of the point.
  \param y : the ordonnee of the point.
  \param col : the collision value of the point.
*/
void
putCollision (Sint32 x, Sint32 y, Sint32 col)
{
  Col[y / height * column + x / width] = col;
}

/**
 * Inits the game datas
 */
bool
init_game (void)
{
  char filename[100];

  player.w = SPOINGW;
  player.h = SPOINGH;
  player.ax = (float) AX;

  /*! Loads the GFX files */
  sprintf (filename, "%smouse.png", REP_GFX);
  IMG_mouse = loadPictureFromZip (PAKNAME, filename);   //IMG_mouse=IMG_Load(filename);
  if (IMG_mouse == NULL)
    {
      return false;
    }
  SDL_SetColorKey (IMG_mouse, SDL_SRCCOLORKEY,
                   SDL_MapRGB (IMG_mouse->format, 8, 36, 107));

  /*! Inits the screen */
  sprintf (filename, "%sIcon.png", REP_GFX);
  screen = initSDL ("Hopy One", filename);

  sprintf (filename, "%scable.png", REP_GFX);
  IMG_cable = loadPictureFromZip (PAKNAME, filename);   //IMG_cable=IMG_Load(filename);
  if (IMG_cable == NULL)
    {
      return false;
    }

  sprintf (filename, "%stir.png", REP_GFX);
  IMG_fire = loadPictureFromZip (PAKNAME, filename);    //IMG_Load(filename);
  if (IMG_fire == NULL)
    {
      return false;
    }
  SDL_SetColorKey (IMG_fire, SDL_SRCCOLORKEY,
                   SDL_MapRGB (IMG_mouse->format, 4, 2, 4));

  sprintf (filename, "%scorps.png", REP_GFX);
  IMG_body = loadPictureFromZip (PAKNAME, filename);    //IMG_Load(filename);
  if (IMG_body == NULL)
    {
      return false;
    }

  sprintf (filename, "%sTitlePic.png", REP_GFX);
  //sprintf(filename, "%scable.png", REP_GFX);
  fond_menu = loadPictureFromZip (PAKNAME, filename);   //loadPicture(filename);
  if (fond_menu == NULL)
    {
      return false;
    }

  sprintf (filename, "%sgameover.png", REP_GFX);
  //sprintf(filename, "%scable.png", REP_GFX);
  gameover = loadPictureFromZip (PAKNAME, filename);    //loadPicture(filename);
  if (gameover == NULL)
    {
      return false;
    }


  /*! Loads the datas for the rope */
  loadRope ();

  /*! Inits the sound mode */
  initSound ();

  /*! Loads the sounds */
  openSound (PAKNAME, REP_SND "rifle1.wav", 0);
  openSound (PAKNAME, REP_SND "launch1.wav", 1);
  openSound (PAKNAME, REP_SND "explode.wav", 2);
  openSound (PAKNAME, REP_SND "tick.wav", 3);
  openSound (PAKNAME, REP_SND "bonus.wav", 4);
  openSound (PAKNAME, REP_SND "noselect.wav", 5);
  openSound (PAKNAME, REP_SND "jump.wav", 6);

  /*! Loads the high scores of the game */
  loadHighScores ();

  /*! Checks for joystick and open it */
  if (SDL_NumJoysticks () > 0)
    {
      /*! Open the joystick */
      joy = SDL_JoystickOpen (0);

      if (joy)
        {
          /*fprintf(stdout, "Opened Joystick 0\n");
             fprintf(stdout, "Name: %s\n", SDL_JoystickName(0));
             fprintf(stdout, "Number of Axes: %d\n", SDL_JoystickNumAxes(joy));
             fprintf(stdout, "Number of Buttons: %d\n", SDL_JoystickNumButtons(joy));
             fprintf(stdout, "Number of Balls: %d\n", SDL_JoystickNumBalls(joy)); */
        }
      else
        fprintf (stderr, "Couldn't open Joystick 0\n");
    }
  return 1;
}

/*! This function applies the changes due to the death of the player and plays the animations */
void
Die (void)
{
  Sint32 i, j;

  if (die == 0)
    {
      if (form == SPOINGFORM)
        {
          die = LIFE_BIGBURST;
          for (i = (int) player.x; i < (int) player.x + player.w; i += 32)
            for (j = (int) player.y;
                 j < (int) player.y + player.h && j + 32 < line * height;
                 j += 32)
              addBigBurst (i, j);
        }
      else
        {
          die = LIFE_BURST;
          for (i = (int) player.x; i < (int) player.x + player.w; i += 16)
            for (j = (int) player.y;
                 j < (int) player.y + player.h && j + 16 < line * height;
                 j += 16)
              addBurst (i, j);
        }
    }
}

/*! This function reinitializes the game for a new play*/
void
restart (void)
{
  player.life--;

  initMap ();

  first = 3;
  actu_score = actu_ammo = actu_life = actu_time = 3;

  //die=0;
}

/**
 * Frees all the game allocations
 */
void
freeGame (void)
{
  SDL_FreeSurface (screen);
  SDL_FreeSurface (IMG_map);
  SDL_FreeSurface (IMG_decor);
  SDL_FreeSurface (IMG_old_decor);
  SDL_FreeSurface (SDLfont);
  SDL_FreeSurface (IMG_spoing);
  TTF_CloseFont (TTFfont);
  TTF_CloseFont (TTFfontmenu);
  TTF_CloseFont (TTFfontmenu2);
  if (TTF_WasInit ())
    {
      TTF_Quit ();
    }
  cleanSound ();
  if (SDL_JoystickOpened (0))
    {
      SDL_JoystickClose (joy);
    }
}

/*! Loads all the datas for a level*/
void
initMap (void)
{
  Sint32 i;
  char fichier_map[100], fichier_spoing[100];

//CHARGEMENT DE TOUT LE CONTENU DU FICHIER COL

  /*! Loads collisions */
  loadCol ();

  apparition = 1;

  /*! Load spoing's coordinates */
  player.x = Col[line * column] * width;
  player.y = Col[line * column + 1] * height;

  /*! Load doors */
  for (i = 0; i < 10; i++)
    {
      Door[i][0] = (Col[line * column + 16 + 8 * i] == 11 ? 1 : 0);
      Door[i][1] = Col[line * column + 17 + 8 * i];
    }

  loadCollapsingBlocks ();

  /*! Load ennemys */
  loadEnnemys ();

  /*! Load shoots */
  loadShoots ();

  //On récupére le nom du fichier spoing
  //tableauCol[line*column+112] contenant le numero de la page du spoing..
  sprintf (fichier_spoing, "%sSPOING%02d.png", REP_GFX,
           Col[line * column + 112]);

  //On récupére le nom du fichier décor,
  //tableauCol[line*column+113] contenant le numero de la page de decor...
  sprintf (fichier_map, "%sFONDSP%02d.png", REP_GFX,
           Col[line * column + 113]);

//CHARGEMENT DES FICHIERS GRAPHIQUES

  SDL_Surface *tmp;

  if (IMG_spoing != NULL)
    SDL_FreeSurface (IMG_spoing);

  tmp = loadPicture (fichier_spoing);
  IMG_spoing = SDL_DisplayFormat (tmp);
  SDL_FreeSurface (tmp);

  SDL_SetColorKey (IMG_spoing, SDL_SRCCOLORKEY,
                   SDL_MapRGB (IMG_spoing->format, 4, 2, 4));

  if (IMG_map != NULL)
    SDL_FreeSurface (IMG_map);
  tmp = loadPicture (fichier_map);
  IMG_map = SDL_DisplayFormat (tmp);
  SDL_FreeSurface (tmp);

//CHARGEMENT DE LA MAP

  //On charge le tableau de decor
  loadMap ();

  //Selon l'etat du niveau (si on a deja pris tous les bonus)
  //On actualise le nb de bonus a prendre
  NbBonus = 0;
  for (i = 0; i < line * column; i++)
    {
      if (!Bonus[player.level])
        {
          if (Col[i] == COL_BONUS)
            NbBonus++;
        }
      else
        {
          if (Col[i] == COL_BONUS)
            {
              Map[i * 2] = X - width;
              Map[i * 2 + 1] = 0;
              //copyPicture(map, decor, 640-width, 0, width, height, i/column, i-i/column);
              Col[i] = COL_VIDE;
            }
        }
    }

  player.undying = UNDYING;

  for (i = 0; i < NBSHOOTS; i++)
    playerShoot[i].state = 0;
  for (i = 0; i < NBSHOOTSE; i++)
    detectingShoot[i].state = 0;
  for (i = 0; i < NBSHOOTSE; i++)
    ennemyShoot[i].state = 0;

  for (i = 0; i < NB_BURSTS; i++)
    burst[i].life = -1;
  for (i = 0; i < NB_BIGBURSTS; i++)
    bigburst[i].life = -1;

  initStar ();

  //On copie le decor dans "decor", a partir de la map et du tableau de decor
  //Ceci sert pour l'affichage de la map dans le jeu,
  //Mais vu qu'ici la map peut etre constamment changé, on la réaffiche a chaque fois...
  initDecor ();

//CHARGEMENT DU SPOING
  loadSpoing ();

  player.dx = 0;
  player.dy = 0;
  CableLaunched = 0;

  if (form == SPOINGFORM)
    player.h = SPOINGH;
}

/*! Displays the player, in spoing form*/
void
displayPlayer (void)
{
#ifdef debug
  displayEmptySquare (screen, (int) player.x, (int) player.y, player.w, HEADH,
                      200, 0, 0);
  displayEmptySquare (screen, (int) player.x + 11, (int) player.y + HEADH,
                      player.w - 22, player.h - HEADH - 12, 0, 200, 0);
  displayEmptySquare (screen, (int) player.x, (int) player.y + player.h - 12,
                      player.w, 12, 0, 0, 200);

//       displayEmptySquare(screen, player.x, player.y, player.w, player.h, 200, 200, 200);
#else
  if (form == SPOINGFORM)
    {
      if (player.undying % 2 == 0 || player.undying == -1)
        {
          //Copie la tete
          copyPicture (IMG_spoing, screen, 0, 0, player.w, HEADH,
                       (int) player.x, (int) player.y);
          //copie le corps
          copyPicture (IMG_body, screen, 0, 0, player.w,
                       player.h - HEADH - 12, (int) player.x + 11,
                       (int) player.y + HEADH);
          //Copie les pieds
          copyPicture (IMG_spoing, screen, 0, FOOTH, player.w, 12,
                       (int) player.x, (int) player.y + player.h - 12);
        }
    }
  else
    {
      if (player.undying % 2 == 0 || player.undying == -1)
        copyPicture (IMG_spoing, screen, 399, 0, player.w, player.h,
                     (int) player.x, (int) player.y);
    }
#endif

  displayEmptySquare (screen, 0, 2, (int) (saut / AY * 50) + 1, 6, 0, 0, 0);
  displayFullSquare (screen, 1, 3, (int) (saut / AY * 50), 5, 100, 100, 100);
}

void
displayMap (void)
{
  SDL_BlitSurface (IMG_decor, NULL, screen, NULL);
}

void
loadCol (void)
{
  char filename[100];
  unsigned char tableau[line * column * 2];
  Sint32 i;


  sprintf (filename, "%sSPOING%02d.COL", REP_COL, player.level);

  /*if ((fichier=fopen(filename,"rb"))==NULL)
     {
     fprintf(stderr, "impossible de load le fichier %s !\n", filename);
     exit(2);
     }
     else */
  //fichier=loadFileFromZip(PAKNAME, filename);
  SDL_RWops *op;
  op = TLK_RWFromZip (PAKNAME, filename);
  if (op != 0)
    {
      SDL_RWread (op, &tableau, sizeof (unsigned char), line * column * 2);
      op->close (op);
    }
  else
    {
      fprintf (stderr, "Error while trying to load %s/%s\n", PAKNAME,
               filename);
      exit (1);
    }

  /*{ 
     fread(tableau, line*column*2, sizeof(unsigned char),fichier);
     fclose(fichier);
     } */

  for (i = 0; i < line * column * 2; i++)
    Col[i] = tableau[i];
}

void
loadEnnemys (void)
{
  Sint32 i;

  for (i = 0; i < NBSHOOTSE; i++)
    {
      ennemy[i].x = Col[line * column + 120 + i * 8] * width;
      ennemy[i].y = Col[line * column + 120 + i * 8 + 1] * height;
      ennemy[i].direction = Col[line * column + 120 + i * 8 + 2];
      ennemy[i].cadence_tir = Col[line * column + 120 + i * 8 + 3];
      ennemy[i].type_tir = Col[line * column + 120 + i * 8 + 4];
      ennemy[i].resistance = Col[line * column + 120 + i * 8 + 5];
      ennemy[i].bonus = Col[line * column + 120 + i * 8 + 6];
      ennemy[i].kind = Col[line * column + 120 + i * 8 + 7];
      ennemy[i].sens = -1;
      ennemy[i].w = ennemy[i].kind == 5 ? 32 : 36;      //23 pour 640*480
      ennemy[i].h = ennemy[i].kind == 5 ? 10 : 46;      //23
    }
}

void
loadShoots (void)
{
  Sint32 i;

  for (i = 0; i < NBSHOOTSE; i++)
    {
      detectingArea[i].x = Col[line * column + 200 + i * 8] * width;
      detectingArea[i].y = Col[line * column + 200 + i * 8 + 1] * height;
      detectingArea[i].cad = Col[line * column + 200 + i * 8 + 2];

      detectingArea[i].dx = Col[line * column + 280 + i * 8 + 0] * width;
      detectingArea[i].dy = Col[line * column + 280 + i * 8 + 1] * height;
      detectingArea[i].debx = Col[line * column + 280 + i * 8 + 2] * width;
      detectingArea[i].deby = Col[line * column + 280 + i * 8 + 3] * height;
      detectingArea[i].finx = Col[line * column + 280 + i * 8 + 4] * width;
      detectingArea[i].finy = Col[line * column + 280 + i * 8 + 5] * height;
    }
}

void
loadMap (void)
{
  char filename[100];
  unsigned char tableau[line * column * 2];
  Sint32 i;


  sprintf (filename, "%sSPOING%02d.MAP", REP_MAP, player.level);

  /*if ((fichier=fopen(filename,"rb"))==NULL)
     {
     fprintf(stderr, "impossible de load le fichier %s !\n", filename);
     exit(2);
     }
     else */
  SDL_RWops *op;
  op = TLK_RWFromZip (PAKNAME, filename);
  if (op != 0)
    {
      SDL_RWread (op, &tableau, sizeof (unsigned char), line * column * 2);
      op->close (op);
    }
  else
    {
      fprintf (stderr, "Error while trying to load %s/%s\n", PAKNAME,
               filename);
      exit (1);
    }

  /*fichier=loadFileFromZip(PAKNAME, filename);
     { 
     fread(tableau, line*column*2, sizeof(unsigned char),fichier);
     fclose(fichier);
     } */

  //Inversion, et remise a l'echelle graphique
  for (i = 0; i < line * column * 2; i += 2)
    {
      Map[i] = (tableau[i + 1] / 2) * width;
      Map[i + 1] = tableau[i] / 10 * height;
    }
}

void
loadSpoing (void)
{
  unsigned char tableau[390];
  Sint32 i;
  char filename[100];


  sprintf (filename, "poxpixsp.dat");
  /*if ((fichier=fopen(filename,"rb"))==NULL)
     {
     fprintf(stderr, "impossible de load le fichier %s !\n", filename);
     exit(2);
     }
     else */
  /*fichier=loadFileFromZip(PAKNAME, filename);
     { 
     fread(tableau, 390, sizeof(unsigned char),fichier);
     fclose(fichier);
     } */
  SDL_RWops *op;
  op = TLK_RWFromZip (PAKNAME, filename);
  if (op != 0)
    {
      SDL_RWread (op, &tableau, sizeof (unsigned char), 390);
      op->close (op);
    }
  else
    {
      fprintf (stderr, "Error while trying to load %s/%s\n", PAKNAME,
               filename);
      exit (1);
    }


  for (i = 0; i < 390; i++)
    arrayXSpoing[i] = tableau[i];

  sprintf (filename, "poypixsp.dat");
  /*if ((fichier=fopen(filename,"rb"))==NULL)
     {
     fprintf(stderr, "impossible de load le fichier %s !\n", filename);
     exit(2);
     }
     else */
  /*fichier=loadFileFromZip(PAKNAME, filename);
     { 
     fread(tableau, 390, sizeof(unsigned char),fichier);
     fclose(fichier);
     } */
  op = TLK_RWFromZip (PAKNAME, filename);
  if (op != 0)
    {
      SDL_RWread (op, &tableau, sizeof (unsigned char), 390);
      op->close (op);
    }
  else
    {
      fprintf (stderr, "Error while trying to load %s/%s\n", PAKNAME,
               filename);
      exit (1);
    }


  for (i = 0; i < 390; i++)
    arrayYSpoing[i] = tableau[i];
}

void
initDecor (void)
{
  Sint32 i, j;

  if (IMG_old_decor == NULL)
    IMG_old_decor =
      SDL_CreateRGBSurface (SDL_SWSURFACE, width * column, height * line,
                            COLOR_DEPTH, 0, 0, 0, 0);

  SDL_BlitSurface (IMG_decor, NULL, IMG_old_decor, NULL);
  SDL_FreeSurface (IMG_decor);

  IMG_decor =
    SDL_CreateRGBSurface (SDL_SWSURFACE | SDL_SRCALPHA, width * column,
                          height * line, COLOR_DEPTH, 0, 0, 0, 0);

  //Affiche le decor case par case (1 case = width*height pixls)
  for (i = 0; i < column; i++)
    for (j = 0; j < line; j++)
      {
        displayCase (IMG_decor, i, j);
        displayShadow (IMG_decor, i, j);
      }


  displayCollapsingBlocks ();

}

void
displayCase (SDL_Surface * surface, Sint32 i, Sint32 j)
{
  SDL_Rect src, dest;

  src.w = width;
  src.h = height;

  src.x = Map[(j * column + i) * 2];
  src.y = Map[(j * column + i) * 2 + 1];

  dest.x = i * width;
  dest.y = j * height;

  SDL_BlitSurface (IMG_map, &src, surface, &dest);

}

void
displayShadow (SDL_Surface * surface, Sint32 i, Sint32 j)
{
  SDL_Rect src, dest;

  Uint32 shadow_color = SDL_MapRGBA (surface->format, 0, 0, 0, 0);

  SDL_Surface *tmp =
    SDL_CreateRGBSurface (SDL_SWSURFACE | SDL_SRCALPHA, width, height,
                          COLOR_DEPTH, 0, 0, 0, 0);
  SDL_FillRect (tmp, NULL, shadow_color);
  SDL_SetAlpha (tmp, SDL_SRCALPHA, SHADOW_LEVEL);

  if (Col[j * column + i] == COL_VIDE)
    {
      if (i > 0 && j > 0)
        {
          src.w = width / 2;
          src.h = height / 2;
          src.x = 0;
          src.y = 0;

          if (Col[j * column + i - 1] > 98)
            {
              src.w = width / 2;

              //src.x=i*width;
              //src.y=j*height+height/2;
              dest.x = i * width;
              dest.y = j * height + height / 2;

              SDL_BlitSurface (tmp, &src, surface, &dest);
              //SDL_FillRect(surface, &src, shadow_color);
            }

          if (Col[(j - 1) * column + i - 1] > 98)
            {
              src.w = width / 2;

//                src.x=i*width;
//                src.y=j*height;
              dest.x = i * width;
              dest.y = j * height;

              SDL_BlitSurface (tmp, &src, surface, &dest);
              //SDL_FillRect(surface, &src, shadow_color);
            }

          if (Col[(j - 1) * column + i] > 98)
            {
              src.w = width / 2 + 1;

              //src.x=i*width+width/2;
              //src.y=j*height;
              dest.x = i * width + width / 2;
              dest.y = j * height;

              //SDL_FillRect(surface, &src, shadow_color);
              SDL_BlitSurface (tmp, &src, surface, &dest);
            }
        }
    }
}

void
initFonts (void)
{


  if (TTF_Init () != 0)
    {
      fprintf (stderr, "impossible d'initialiser TTF ! \n%s",
               SDL_GetError ());
      exit (2);
    }

  TTFfont = loadFontFromZip (PAKNAME, REP_FONT "vagron.ttf", 30);
  TTFfontmenu = loadFontFromZip (PAKNAME, REP_FONT "vagron.ttf", 16);
  TTFfontmenu2 = loadFontFromZip (PAKNAME, REP_FONT "vagron.ttf", 19);

  TTF_SetFontStyle (TTFfontmenu2, TTF_STYLE_BOLD);
}

void
openDoors (void)
{
  if (Door[0][0])
    copyPicture (IMG_map, IMG_decor, width * column - width, 8 * height,
                 width, 5 * height, 0, height * 6);
  if (Door[1][0])
    copyPicture (IMG_map, IMG_decor, width * column - width, 8 * height,
                 width, 5 * height, 0, height * 17);
  if (Door[2][0])
    copyPicture (IMG_map, IMG_decor, width * column - width, 8 * height,
                 width, 5 * height, 0, height * 29);
  if (Door[3][0])
    copyPicture (IMG_map, IMG_decor, 35 * width, 15 * height, 5 * width,
                 height, width * 8, 34 * height);
  if (Door[4][0])
    copyPicture (IMG_map, IMG_decor, 35 * width, 15 * height, 5 * width,
                 height, width * 27, 34 * height);
  if (Door[5][0])
    copyPicture (IMG_map, IMG_decor, width * column - width, 8 * height,
                 width, 5 * height, width * 39, height * 29);
  if (Door[6][0])
    copyPicture (IMG_map, IMG_decor, width * column - width, 8 * height,
                 width, 5 * height, width * 39, height * 17);
  if (Door[7][0])
    copyPicture (IMG_map, IMG_decor, width * column - width, 8 * height,
                 width, 5 * height, width * 39, height * 6);
  if (Door[8][0])
    copyPicture (IMG_map, IMG_decor, 35 * width, 15 * height, 5 * width,
                 height, width * 27, 0);
  if (Door[9][0])
    copyPicture (IMG_map, IMG_decor, 35 * width, 15 * height, 5 * width,
                 height, width * 8, 0);
}

/*! This function updates the texts */
void
displayInfos (void)
{
  static Sint32 nbframes = 0;
  Sint32 precision = 25;
  char buffer[100];

  if (LastTick / 1000 != NewTick / 1000)
    actu_time = 2;

  if (nbframes == precision)
    {
      fps = precision * 1000 / (NewTick - LastTick);
      LastTick = NewTick;
      nbframes = 0;
    }

  nbframes++;

  Sint32 ordonnee = height * line;

  if (actu_score > 0)
    {
      sprintf (buffer, "%s : %08d  ", text[TEXT_SCORE], player.score);
      refreshZone (buffer, 0, line * height, X / 2, (Y - line * height) / 2);
      actu_score--;
    }

  if (actu_time > 0)
    {
      sprintf (buffer, "%s : %02d:%02d  ", text[TEXT_TIME],
               (NewTick / 1000) / 60, (NewTick / 1000) % 60);
      refreshZone (buffer, X / 2, line * height, X / 2,
                   (Y - line * height) / 2);
      actu_time--;
    }

  if (actu_life > 0)
    {
      sprintf (buffer, "%s : %02d  ", text[TEXT_LIFE], player.life);
      refreshZone (buffer, 0, ordonnee + (Y - line * height) / 2, X / 2,
                   (Y - line * height) / 2);
      actu_life--;
    }

  if (actu_ammo > 0)
    {
      sprintf (buffer, "%s : %04d  ", text[TEXT_AMMO], player.ammo);
      refreshZone (buffer, X / 2, ordonnee + (Y - line * height) / 2, X / 2,
                   (Y - line * height) / 2);
      actu_ammo--;
    }

  sprintf (buffer, "%02d", fps);
  printText (screen, TTFfont, buffer, X - 5 * 20, 0, 0, 100, 150);
}

void
displayShoots (void)
{
  Sint32 i;

  for (i = 0; i < NBSHOOTS; i++)
    {
      if (playerShoot[i].state)
        {
#ifdef debug
          displayEmptySquare (screen, playerShoot[i].x, playerShoot[i].y,
                              FireW, FireH, 200, 0, 0);
#else
          copyPicture (IMG_fire, screen, 0, i % 2 != 0 ? 0 : FireH, FireW,
                       FireH, playerShoot[i].x, playerShoot[i].y);
#endif
        }
    }

  for (i = 0; i < NBSHOOTSE; i++)
    {
      if (ennemyShoot[i].state)
        {
#ifdef debug
          displayEmptySquare (screen, ennemyShoot[i].x, ennemyShoot[i].y,
                              FireW, FireH, 200, 0, 0);
#else
          copyPicture (IMG_spoing, screen, 0, 305, 14, 14, ennemyShoot[i].x,
                       ennemyShoot[i].y);
#endif
        }
      if (detectingShoot[i].state)
        {
#ifdef debug
          displayEmptySquare (screen, detectingShoot[i].x,
                              detectingShoot[i].y, FireW, FireH, 200, 0, 0);
#else
          copyPicture (IMG_spoing, screen, 0, 305, 14, 14,
                       detectingShoot[i].x, detectingShoot[i].y);
#endif
        }
    }
}

void
animate (SDL_Surface * surface1, SDL_Surface * surface2, Sint32 direction)
{
  Sint32 i, max;
  SDL_Surface *tmp1, *tmp2;

  tmp1 =
    SDL_CreateRGBSurface (SDL_SWSURFACE, X, surface1->h, COLOR_DEPTH, 0, 0, 0,
                          0);
  tmp1 = SDL_DisplayFormat (surface1);
  tmp2 =
    SDL_CreateRGBSurface (SDL_SWSURFACE, X, surface2->h, COLOR_DEPTH, 0, 0, 0,
                          0);
  tmp2 = SDL_DisplayFormat (surface2);

  if (direction == EVENT_LEFT || direction == EVENT_RIGHT)
    max = X;
  else
    max = tmp1->h;

  Sint32 dx = 4;

  for (i = 0; i < max; i += dx)
    {
      if (i + dx >= max)
        dx = 1;

      switch (direction)
        {
        case EVENT_LEFT:
          copyPicture (tmp1, screen, 0, 0, X + 1 - i, tmp1->h, i, 0);
          copyPicture (tmp2, screen, X - 1 - i, 0, i + 1, tmp1->h, 0, 0);
          break;

        case EVENT_RIGHT:
          copyPicture (tmp1, screen, i, 0, X + 1 - i, tmp1->h, 0, 0);
          copyPicture (tmp2, screen, 0, 0, i + 1, tmp2->h, X - 1 - i, 0);
          break;

        case EVENT_UP:
          copyPicture (tmp1, screen, 0, 0, X, tmp1->h - i, 0, i);
          copyPicture (tmp2, screen, 0, tmp1->h - 1 - i, X, i + 1, 0, 0);
          break;

        case EVENT_DOWN:
          copyPicture (tmp1, screen, 0, i, X, tmp1->h - i, 0, 0);
          copyPicture (tmp2, screen, 0, 0, X, i + 1, 0, tmp1->h - 1 - i);
          break;
        }

      //SDL_Delay(5); //fps/20);

      //fprintf(stdout, "%d", SDL_Flip(screen));
      while (SDL_Flip (screen) == -1);
    }

  SDL_FreeSurface (tmp1);
  SDL_FreeSurface (tmp2);
}

void
refreshZone (char buffer[], Sint32 x, Sint32 y, Sint32 w, Sint32 h)
{
  SDL_Rect rect;

  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;

  SDL_FillRect (screen, &rect, SDL_MapRGB (screen->format, 0, 0, 0));
  printText (screen, TTFfont, buffer, x, y, 100, 0, 0);
}

void
displayEnnemys (void)
{
  Sint32 i;
  SDL_Surface *tmp;

  tmp =
    SDL_CreateRGBSurface (SDL_SWSURFACE, ennemy[0].w, ennemy[0].h,
                          COLOR_DEPTH, 0, 0, 0, 0);
  SDL_SetColorKey (tmp, SDL_SRCCOLORKEY, SDL_MapRGB (tmp->format, 0, 0, 0));

  for (i = 0; i < 10; i++)
    {
      if (ennemy[i].x != 0)
        {
#ifdef debug
          displayEmptySquare (screen, ennemy[i].x, ennemy[i].y, ennemy[i].w,
                              ennemy[i].h, 200, 0, 0);
#else
          SDL_FillRect (tmp, NULL, SDL_MapRGB (tmp->format, 0, 0, 0));
          copyPicture (IMG_spoing, tmp, 0,
                       ennemy[i].sens == 1 ? 46 + ennemy[i].h : 46,
                       ennemy[i].w, ennemy[i].h, 0, 0);

          if (ennemy[i].mask > 0)
            {
              if (ennemy[i].mask % 4 == 0)
                copyPicture (displayMask (tmp), screen, 0, 0, ennemy[i].w,
                             ennemy[i].h, ennemy[i].x, ennemy[i].y);
              else
                copyPicture (tmp, screen, 0, 0, ennemy[i].w, ennemy[i].h,
                             ennemy[i].x, ennemy[i].y);
              ennemy[i].mask--;
            }
          else
            copyPicture (tmp, screen, 0, 0, ennemy[i].w, ennemy[i].h,
                         ennemy[i].x, ennemy[i].y);

          //copyPicture(IMG_spoing, screen, 0, ennemy[i].sens==1 ? 23+ennemy[i].h : 23, ennemy[i].w, ennemy[i].h, ennemy[i].x, ennemy[i].y);
#endif
        }
    }

  SDL_FreeSurface (tmp);
}

void
displayCables (void)
{
#ifdef debug
  if (form == SPOINGFORM)
    {
      displayEmptySquare (screen, (int) XLeftCable, (int) YLeftCable,
                          (int) player.x - (int) XLeftCable, 4, 200, 0, 0);
      displayEmptySquare (screen, (int) player.x + player.w,
                          (int) YRightCable,
                          (int) XRightCable - (int) player.x - player.w, 4,
                          200, 0, 0);
    }
#else
  if (form == SPOINGFORM)
    {
      copyPicture (IMG_cable, screen, (int) XLeftCable % 4, 0,
                   (int) player.x - (int) XLeftCable, 3, (int) XLeftCable,
                   (int) YLeftCable);
      copyPicture (IMG_cable, screen, ((int) player.x + player.w) % 4, 0,
                   (int) XRightCable - (int) player.x - player.w, 3,
                   (int) player.x + player.w, (int) YRightCable);
    }

#endif
}

/*! This function redraw all the texts */
void
displayInfos2 (void)
{
  char buffer[100];

  SDL_Rect rect;

  rect.x = 0;
  rect.y = height * line;
  rect.w = X;
  rect.h = Y - height * line;

  SDL_FillRect (screen, &rect, SDL_MapRGB (screen->format, 0, 0, 0));

  sprintf (buffer, "%s : ", text[TEXT_SCORE]);
  printTextBlended (screen, TTFfont, buffer, 0, height * line, 100, 0, 0);

  sprintf (buffer, "%s : ", text[TEXT_TIME]);
  printTextBlended (screen, TTFfont, buffer, X / 2, height * line, 100, 0, 0);

  sprintf (buffer, "%s : ", text[TEXT_LIFE]);
  printTextBlended (screen, TTFfont, buffer, 0,
                    height * line + (Y - line * height) / 2, 100, 0, 0);

  sprintf (buffer, "%s : ", text[TEXT_AMMO]);
  printTextBlended (screen, TTFfont, buffer, X / 2,
                    height * line + (Y - line * height) / 2, 100, 0, 0);

}

Sint32
testLeftCollisions (void)
{
  Sint32 i, j, k, dx;

  if (form == SPOINGFORM)
    {
      StructCoord head[4] = { {13, 2}
      , {1, 10}
      , {1, 22}
      , {5, 30}
      };                        //{{8,1}, {1,5}, {1,11}, {5,14}};
      StructCoord body = { 11, 0 };     //4,0
      StructCoord tail[2] = { {0, 20}
      , {3, 22}
      };                        //{{0,20}, {2,22}};

      if (player.dx < 0)
        dx = (int) player.dx - 1;
      else
        dx = -1;

      if (getEnnemySpoingCollisions (EVENT_LEFT))       //if (ennemyCollisions(EVENT_LEFT))
        return 1;

      for (k = dx; k < 0; k++)
        {
          // Head collisions
          for (i = 0; i < 4; i++)
            {
              j =
                getCollision ((int) player.x + head[i].x + k,
                              (int) player.y + head[i].y);

              if (isCollision (j))      //if (j!=0 && j!=COL_SECRET && j!=COL_BONUS && j!=COL_TRANS_SB && j!=COL_TRANS_BS)
                return 1;
            }

          // Body collisions
          for (i = HEADH; i < player.h - 12; i++)
            {
              j =
                getCollision ((int) player.x + body.x + k,
                              (int) player.y + i);

              if (isCollision (j))      //if (j!=0 && j!=COL_SECRET && j!=COL_BONUS && j!=COL_TRANS_SB && j!=COL_TRANS_BS)
                return 1;
            }

          // Tail collisions
          for (i = 0; i < 2; i++)
            {
              j =
                getCollision ((int) player.x + tail[i].x + k,
                              (int) player.y + (player.h - 23) + tail[i].y);

              if (isCollision (j))      //if (j!=0 && j!=COL_SECRET && j!=COL_BONUS && j!=COL_TRANS_SB && j!=COL_TRANS_BS)
                return 1;
            }

        }
    }
  else
    {
      StructCoord collisions[7] = { {5, 0}
      , {0, 5}
      , {0, 9}
      , {4, 14}
      , {2, 2}
      , {5, 14}
      , {2, 12}
      };

      if (player.dx < 0)
        dx = (int) player.dx - 1;
      else
        dx = -1;

      if (getEnnemySpoingCollisions (EVENT_LEFT))       //if (ennemyCollisions(EVENT_LEFT))
        return 1;

      for (k = dx; k < 0; k++)
        {
          for (i = 0; i < 7; i++)
            {
              j =
                getCollision ((int) player.x + collisions[i].x + k,
                              (int) player.y + collisions[i].y);

              if (isCollision (j))      //if (j!=0 && j!=COL_SECRET && j!=COL_BONUS && j!=COL_TRANS_SB && j!=COL_TRANS_BS)
                return 1;
            }
        }
    }
  return 0;
}

Sint32
testRightCollisions (void)
{
  Sint32 i, j, k, dx;

  if (form == SPOINGFORM)
    {
      StructCoord head[5] = { {X2 (16), 0}
      , {X2 (23), Y2 (5)}
      , {X2 (15), Y2 (8)}
      , {X2 (23), Y2 (10)}
      , {X2 (15), Y2 (14)}
      };
      StructCoord body = { 27, -1 };    //y doesn't mind
      StructCoord tail[2] = { {X2 (23), 19}
      , {X2 (20), 22}
      };
/*       StructCoord head[5]={{26,0}, {38,8}, {29,12}, {38,18}, {29,22}};
       StructCoord body={29,-1};  //y doesn't mind
       StructCoord tail[2]={{23,19}, {20,22}};
*/
      if (getEnnemySpoingCollisions (EVENT_RIGHT))      //if (ennemyCollisions(EVENT_LEFT))
        return 1;

      if (player.dx > 0)
        dx = (int) player.dx + 1;
      else
        dx = 1;

      for (k = 0; k < dx; k++)
        {
          // Head collisions
          for (i = 0; i < 5; i++)
            {
              j =
                getCollision ((int) player.x + head[i].x + k,
                              (int) player.y + head[i].y);

              if (isCollision (j))      //if (j!=0 && j!=COL_SECRET && j!=COL_BONUS && j!=COL_TRANS_SB && j!=COL_TRANS_BS)
                return 1;
            }

          // Body collisions
          for (i = HEADH; i < player.h - 12; i++)
            {
              j =
                getCollision ((int) player.x + body.x + k,
                              (int) player.y + i);

              if (isCollision (j))      //if (j!=0 && j!=COL_SECRET && j!=COL_BONUS && j!=COL_TRANS_SB && j!=COL_TRANS_BS)
                return 1;
            }

          // Tail collisions
          for (i = 0; i < 2; i++)
            {
              j =
                getCollision ((int) player.x + tail[i].x + k,
                              (int) player.y + (player.h - 23) + tail[i].y);

              if (isCollision (j))      //if (j!=0 && j!=COL_SECRET && j!=COL_BONUS && j!=COL_TRANS_SB && j!=COL_TRANS_BS)
                return 1;
            }
        }
    }
  else
    {
      StructCoord collisions[7] = { {X2 (9), 0}
      , {X2 (12), Y2 (2)}
      , {X2 (14), Y2 (5)}
      , {X2 (14), Y2 (7)}
      , {X2 (14), Y2 (9)}
      , {X2 (12), Y2 (12)}
      , {X2 (9), Y2 (14)}
      };
      //StructCoord collisions[7]={{19,0}, {21,2}, {24,5}, {24,7}, {24,9}, {17,12}, {19,14}};

      if (getEnnemySpoingCollisions (EVENT_RIGHT))      //if (ennemyCollisions(EVENT_LEFT))
        return 1;

      if (player.dx > 0)
        dx = (int) player.dx + 1;
      else
        dx = 1;

      for (k = 0; k < dx; k++)
        {
          for (i = 0; i < 7; i++)
            {
              j =
                getCollision ((int) player.x + collisions[i].x + k,
                              (int) player.y + collisions[i].y);

              if (isCollision (j))      //if (j!=0 && j!=COL_SECRET && j!=COL_BONUS && j!=COL_TRANS_SB && j!=COL_TRANS_BS)
                return 1;
            }
        }
    }

  return 0;
}

Sint32
testUpCollisions (void)
{
  Sint32 i, j, k, dy;

  if (form == SPOINGFORM)
    {
      StructCoord head[6] = { {0, 8}
      , {3, 4}
      , {15, 0}
      , {25, 0}
      , {35, 4}
      , {38, 8}
      };

      StructCoord tail[6] = { {0, 3}
      , {3, 4}
      , {12, 4}
      , {26, 4}
      , {35, 4}
      , {38, 3}
      };

      if (getEnnemySpoingCollisions (EVENT_UP))
        return 1;

      if (player.dy < 0)
        dy = (int) player.dy - 2;
      else
        dy = -2;

      for (k = dy; k < 0; k++)
        {
          //Head collisions
          for (i = 0; i < 6; i++)
            {
              j =
                getCollision ((int) player.x + head[i].x,
                              (int) player.y + head[i].y + k);

              if (isCollision (j))      //if (j!=0 && j!=COL_SECRET && j!=COL_BONUS && j!=COL_TRANS_SB && j!=COL_TRANS_BS)
                return 1;
            }

          //Tail collisions
          for (i = 0; i < 6; i++)
            {
              j =
                getCollision ((int) player.x + tail[i].x,
                              (int) player.y + player.h - 16 + tail[i].y + k);

              if (isCollision (j))      //if (j!=0 && j!=COL_SECRET && j!=COL_BONUS && j!=COL_TRANS_SB && j!=COL_TRANS_BS)
                return 1;
            }

        }

/*       StructCoord collisions[10]={{0,5}, {3,2}, {7,0}, {9,0}, {12,0}, {16,0}, {20,2}, {23,5}, {0,19}, {23,19}};
       
       if (getEnnemySpoingCollisions(EVENT_UP)) //if (ennemyCollisions(EVENT_LEFT))
          return 1;
    
       if (player.dy<0)
          dy=(int) player.dy-2;
       else
          dy=-2;
          
       for (k=dy; k<0; k++)
       {
           for (i=0;i<10;i++)
           {
              if (collisions[i].y>16)
                 j=getCollision((int)player.x+collisions[i].x, (int)player.y+(player.h-22)+collisions[i].y+k);
              else
                 j=getCollision((int)player.x+collisions[i].x, (int)player.y+collisions[i].y+k);
             
              if (isCollision(j)) //if (j!=0 && j!=COL_SECRET && j!=COL_BONUS && j!=COL_TRANS_SB && j!=COL_TRANS_BS)
        	     return 1;		
           }
       }
*/
    }
  else
    {
      StructCoord collisions[7] = { {0, 5}
      , {5, 2}
      , {8, 0}
      , {13, 0}
      , {19, 0}
      , {22, 2}
      , {24, 5}
      };

      if (getEnnemySpoingCollisions (EVENT_UP)) //if (ennemyCollisions(EVENT_LEFT))
        return 1;

      if (player.dy < 0)
        dy = (int) player.dy - 2;
      else
        dy = -2;

      for (k = dy; k < 0; k++)
        {
          for (i = 0; i < 7; i++)
            {
              j =
                getCollision ((int) player.x + collisions[i].x,
                              (int) player.y + collisions[i].y + k);

              if (isCollision (j))      //if (j!=0 && j!=COL_SECRET && j!=COL_BONUS && j!=COL_TRANS_SB && j!=COL_TRANS_BS)
                return 1;
            }
        }
    }

  return 0;
}

Sint32
testUpCollisions2 (void)
{
  Sint32 i, j, k, dy;

  if (form == SPOINGFORM)
    {
      StructCoord tail[6] = { {0, 3}
      , {3, 4}
      , {12, 4}
      , {26, 4}
      , {35, 4}
      , {38, 3}
      };

      if (getEnnemySpoingCollisions (EVENT_UP))
        return 1;

      if (player.dy < 0)
        dy = (int) player.dy - 2;
      else
        dy = -2;

      for (k = dy; k < 0; k++)
        {
          //Tail collisions
          for (i = 0; i < 6; i++)
            {
              j =
                getCollision ((int) player.x + tail[i].x,
                              (int) player.y + player.h - 16 + tail[i].y + k);

              if (isCollision (j))      //if (j!=0 && j!=COL_SECRET && j!=COL_BONUS && j!=COL_TRANS_SB && j!=COL_TRANS_BS)
                return 1;
            }

        }
    }
  else
    {
      return testUpCollisions ();
    }

  return 0;
}

Sint32
testDownCollisions (void)
{
  Sint32 i, j, k, dy;

  if (form == SPOINGFORM)
    {
//       StructCoord collisions[11]={{0,21}, {3,22}, {9,22}, {14,22}, {20,22}, {26,22}, {32,22},
//                                   {38,22}, {37,19}, {0,16}, {38,16}};

      StructCoord head[4] = { {0, 27}
      , {3, 27}
      , {35, 27}
      , {38, 27}
      };

      StructCoord tail[6] = { {0, 12}
      , {6, 12}
      , {11, 12}
      , {21, 12}
      , {31, 12}
      , {38, 12}
      };

      if (getEnnemySpoingCollisions (EVENT_DOWN))
        return 1;

      if (player.dy > 0)
        dy = (int) player.dy + 1;
      else
        dy = 1;

      for (k = 0; k < dy; k++)
        {
          //Head collisions
          for (i = 0; i < 4; i++)
            {
              j =
                getCollision ((int) player.x + head[i].x,
                              (int) player.y + head[i].y + k);

              if (isCollision (j))      //if (j!=0 && j!=COL_SECRET && j!=COL_BONUS && j!=COL_TRANS_SB && j!=COL_TRANS_BS)
                return 1;
            }

          //Tail collisions
          for (i = 0; i < 6; i++)
            {
              j =
                getCollision ((int) player.x + tail[i].x,
                              (int) player.y + player.h - 12 + tail[i].y + k);

              if (isCollision (j))      //if (j!=0 && j!=COL_SECRET && j!=COL_BONUS && j!=COL_TRANS_SB && j!=COL_TRANS_BS)
                return 1;
            }
        }
    }
  else
   {
    return testGroundCollisions ();
   }

  return 0;
}

Sint32
testGroundCollisions (void)
{
  Sint32 i, j, k, dy;

  if (form == SPOINGFORM)
    {

      StructCoord head[4] = { {0, 27}
      , {3, 27}
      , {35, 27}
      , {38, 27}
      };

      StructCoord tail[6] = { {0, 12}
      , {6, 12}
      , {11, 12}
      , {21, 12}
      , {31, 12}
      , {38, 12}
      };

      if (getEnnemySpoingCollisions (EVENT_DOWN))
        return 1;

      if (player.dy > 0)
        dy = (int) player.dy + 2;
      else
        dy = 2;

      for (k = 0; k < dy; k++)
        {
          //Head collisions
          for (i = 0; i < 4; i++)
            {
              j =
                getCollision ((int) player.x + head[i].x,
                              (int) player.y + head[i].y + k);

              if (isCollision (j))      //if (j!=0 && j!=COL_SECRET && j!=COL_BONUS && j!=COL_TRANS_SB && j!=COL_TRANS_BS)
                return 1;
            }

          //Tail collisions
          for (i = 0; i < 6; i++)
            {
              j =
                getCollision ((int) player.x + tail[i].x,
                              (int) player.y + player.h - 12 + tail[i].y + k);

              if (isCollision (j))      //if (j!=0 && j!=COL_SECRET && j!=COL_BONUS && j!=COL_TRANS_SB && j!=COL_TRANS_BS)
                return 1;
            }

        }
    }
  else
    {
      StructCoord collisions[7] = { {0, 25}
      , {2, 27}
      , {5, 29}
      , {7, 29}
      , {9, 29}
      , {12, 28}
      , {14, 26}
      };

      if (getEnnemySpoingCollisions (EVENT_DOWN))
        return 1;

      if (player.dy > 0)
        dy = (int) player.dy + 2;
      else
        dy = 2;

      for (k = 0; k < dy; k++)
        {
          for (i = 0; i < 7; i++)
            {
              j =
                getCollision ((int) player.x + collisions[i].x,
                              (int) player.y + collisions[i].y + k);

              if (isCollision (j))      //if (j!=0 && j!=COL_SECRET && j!=COL_BONUS && j!=COL_TRANS_SB && j!=COL_TRANS_BS)
                return 1;
            }
        }
    }

  return 0;
}

/*
Sint32 testGroundCollisions()
{
       for (k=0; k<dy; k++)
       {
           for (i=0;i<9;i++)
           {
              if (collisions[i].y>16)
                 j=getCollision((int)player.x+collisions[i].x, (int)player.y+(player.h-22)+collisions[i].y+k);
              else
                 j=getCollision((int)player.x+collisions[i].x, (int)player.y+collisions[i].y+k);
             
              if (isCollision(j)) //if (j!=0 && j!=COL_SECRET && j!=COL_BONUS && j!=COL_TRANS_SB && j!=COL_TRANS_BS)
        	     return 1;		
           }
       }
   }
   else
   {
       StructCoord collisions[7]={{0,25}, {2,27}, {5,29}, {7,29}, {9,29}, {12,28}, {14,26}};
       
       if (getEnnemySpoingCollisions(EVENT_DOWN)) 
          return 1;
    
       if (player.dy>0)
          dy=(int) player.dy+2;
       else
          dy=2;
          
       for (k=0; k<dy; k++)
       {
           for (i=0;i<7;i++)
           {
              j=getCollision((int)player.x+collisions[i].x, (int)player.y+collisions[i].y+k);
             
              if (isCollision(j)) //if (j!=0 && j!=COL_SECRET && j!=COL_BONUS && j!=COL_TRANS_SB && j!=COL_TRANS_BS)
        	     return 1;		
           }
       }
   }
   
   
   return 0;
}*/
