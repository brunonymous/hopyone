/**
 * @file collisions.c 
 * @brief Main function is where the program starts execution 
 * @created 2007-11-12 
 * @date 2007-11-12
 */
/*
 * copyright (c) 1998-2007 TLK Games all rights reserved
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

/**************************************************
                    FUNCTIONS
**************************************************/

//! This function return 1 if there is a collision between a point and a square and 0 if not.
/*!
  \param x1 the abscissa of the point.
  \param y1 the ordonnee of the point.
  \param x2 the abscissa of the square.
  \param y2 the ordonnee of the square.
  \param w the width of  the square.
  \param h the height of the square.
*/
int
collisionPointSquare (int x1, int y1, int x2, int y2, int w, int h)
{
  if (x1 > x2 && x1 < x2 + w && y1 > y2 && y1 < y2 + h)
    return 1;
  else
    return 0;
}

//! This function return 1 if there is a collision between 2 squares and 0 if not.
/*!
  \param x1 the abscissa of the 1st square.
  \param y1 the ordonnee of the 1st square.
  \param w1 the width of the 1st square.
  \param h1 the height of the 1st square.
  \param x2 the abscissa of the 2nd square.
  \param y2 the ordonnee of the 2nd square.
  \param w2 the width of the 2nd square.
  \param h2 the height of the 2nd square.
*/
int
collisionSquareSquare (int x1, int y1, int w1, int h1, int x2, int y2, int w2,
                       int h2)
{
  if (x1 + w1 <= x2 || x2 + w2 <= x1 || y1 + h1 <= y2 || y2 + h2 <= y1)
    return 0;

  return 1;
}

//! This function return 1 if there is a collision between 2 squares and 0 if not.
/*!
  \param x1 the abscissa of the 1st square.
  \param y1 the ordonnee of the 1st square.
  \param w1 the width of the 1st square.
  \param h1 the height of the 1st square.
  \param x2 the abscissa of the 2nd square.
  \param y2 the ordonnee of the 2nd square.
  \param w2 the width of the 2nd square.
  \param h2 the height of the 2nd square.
*/
int
testCollisions (int x1, int y1, int w1, int h1, int x2, int y2, int w2,
                int h2)
{
  return (collisionSquareSquare (x1, y1, w1, h1, x2, y2, w2, h2));
}

//! This function updates the dx/dy values of an ennemy shoot, with the Bresenham algo.
/*!
* \param Speed : The speed of the shoot
* \param nbShoot : the number of the shoot
*/
int
manageBresenhamShoots (int Speed, int nbShoot)
{
  int x2, y2, DeltaX, DeltaY, s1, s2, Temp, Interchange, E, i;

  x2 = (int) player.x;
  y2 = (int) player.y;

  s1 = (x2 - ennemyShoot[nbShoot].x);
  s2 = (y2 - ennemyShoot[nbShoot].y);

  DeltaX = abs (s1);
  DeltaY = abs (s2);

  ennemyShoot[nbShoot].dx = 0;
  ennemyShoot[nbShoot].dy = 0;

  if (s1 < 0)
    {
      s1 = -1;
    }
  if (s1 > 0)
    {
      s1 = 1;
    }
  if (s2 < 0)
    {
      s2 = -1;
    }
  if (s2 > 0)
    {
      s2 = 1;
    }

  if (DeltaY > DeltaX)
    {
      Temp = DeltaX;
      DeltaX = DeltaY;
      DeltaY = Temp;
      Interchange = 1;
    }
  else
    Interchange = 0;

  E = 2 * DeltaY - DeltaX;

  for (i = 1; (i <= DeltaX && i < Speed); i++)
    {
      while (E >= 0)
        {
          if (Interchange == 1)
            {
              ennemyShoot[nbShoot].dx += s1;
            }
          else
            {
              ennemyShoot[nbShoot].dy += s2;
            }
          E = E - 2 * DeltaX;
        }

      if (Interchange == 1)
        {
          ennemyShoot[nbShoot].dy += s2;
        }
      else
        {
          ennemyShoot[nbShoot].dx += s1;
        }
      E = E + 2 * DeltaY;
    }

  return (0);
}

int
collision (int x, int y)
{
  if (getCollision (x, y) == COL_VIDE || getCollision (x, y) == COL_BONUS ||
      getCollision (x, y) == COL_TRANS_SB
      || getCollision (x, y) == COL_TRANS_BS
      || getCollision (x, y) == COL_SECRET
      || (getCollision (x, y) >= COL_PORTE_0
          && getCollision (x, y) <= COL_PORTE_9 && NbBonus == 0))
    return 0;
  else
    return 1;
}

int
manageCollisions (int type, int cote, int x, int y, int w, int h)
{
  int i, j;

  switch (cote)
    {
    case EVENT_RIGHT:
      for (i = x / width * width; i <= (x + w) / width * width; i += width)
        for (j = (y + 1) / height * height; j <= (y + h) / height * height;
             j += height)
          if (collision (i, j) || i > X || (type == 1 && getEnnemySpoingCollisions (EVENT_RIGHT)))      //getCollisionennemy(i, j)))
            return 1;
      break;

    case EVENT_LEFT:
      for (i = x / width * width; i >= (x + w) / width * width; i -= width)
        for (j = (y + 1) / height * height; j <= (y + h) / height * height;
             j += height)
          if (collision (i, j) || i < (type == 1 ? 0 : 1) || (type == 1 && getEnnemySpoingCollisions (EVENT_LEFT)))     //getCollisionennemy(i, j)))
            return 1;
      break;

    case EVENT_DOWN:
      for (i = x / width * width; i <= (x + w) / width * width; i += width)
        for (j = (y) / height * height; j <= (y + h) / height * height;
             j += height)
          if (collision (i, j) || j > Y || (type == 1 && getEnnemySpoingCollisions (EVENT_DOWN)))       //getCollisionennemy(i, j)))
            return 1;
      break;

    case EVENT_UP:
      for (i = x / width * width; i <= (x + w) / width * width; i += width)
        for (j = y / height * height; j >= (y + h) / height * height;
             j -= height)
          if (collision (i, j) || j < 0 || (type == 1 && getEnnemySpoingCollisions (EVENT_UP))) //getCollisionennemy(i, j)))
            return 1;
      break;

    }

  return 0;
}


void
win ()
{
  clearSurface (screen);


  SDL_Flip (screen);

}

void
testDoors ()
{
  int i, j;
  int done = 0;

  if (NbBonus == 0)
    {
      for (i = (int) player.x / width * width;
           i <= ((int) player.x + player.w) / width * width && !done;
           i += width)
        for (j = (int) player.y / height * height;
             j <= ((int) player.y + player.h) / height * height && !done;
             j += height)
          if (getCollision (i, j) >= COL_PORTE_0
              && getCollision (i, j) <= COL_PORTE_9)
            {
              if (Door[getCollision (i, j) - COL_PORTE_0][0])
                {
                  player.level = Door[getCollision (i, j) - COL_PORTE_0][1];

                  //Game Over !!!!
                  if (player.level == 79)
                    win ();

                  int porte = getCollision (i, j) - COL_PORTE_0;

                  initMap ();


                  switch (porte)
                    {
                    case 0:
                    case 1:
                    case 2:
                      animate (IMG_old_decor, IMG_decor, EVENT_LEFT);
                      break;
                    case 3:
                    case 4:
                      animate (IMG_old_decor, IMG_decor, EVENT_DOWN);
                      break;
                    case 5:
                    case 6:
                    case 7:
                      animate (IMG_old_decor, IMG_decor, EVENT_RIGHT);
                      break;
                    case 8:
                    case 9:
                      animate (IMG_old_decor, IMG_decor, EVENT_UP);
                      break;
                    }

                  done = 1;
                }
            }
    }
}

void
testMortal ()
{
  int i, j;

  for (i = ((int) player.x - abs ((int) player.dx)) / width * width;
       i <=
       ((int) player.x + player.w + abs ((int) player.dx)) / width * width;
       i += width)
    for (j = ((int) player.y - abs ((int) player.dy)) / height * height;
         j <=
         ((int) player.y + player.h + abs ((int) player.dy) +
          1) / height * height; j += height)
      if (getCollision (i, j) == COL_MORTEL && player.undying == 0)
        {
          Die ();
        }
}

int
getEnnemySpoingCollisions (int cote)
{
  int i, j, n;

  for (n = 0; n < NBENNEMYS; n++)
    {
      if (ennemy[n].x != 0)
        {
          switch (cote)
            {
            case EVENT_RIGHT:
              for (i = (int) player.x + player.w;
                   i <= (int) player.x + player.w + (int) player.dx + 1; i++)
                for (j = (int) player.y + 1; j < (int) player.y + player.h;
                     j++)
                  if (i >= ennemy[n].x && i <= ennemy[n].x + ennemy[n].w
                      && j >= ennemy[n].y && j <= ennemy[n].y + ennemy[n].h)
                    return 1;
              break;
            case EVENT_LEFT:
              for (i = (int) player.x;
                   i >= (int) player.x + (int) player.dx - 1; i--)
                for (j = (int) player.y + 1; j < (int) player.y + player.h;
                     j++)
                  if (i >= ennemy[n].x && i <= ennemy[n].x + ennemy[n].w
                      && j >= ennemy[n].y && j <= ennemy[n].y + ennemy[n].h)
                    return 1;
              break;
            case EVENT_DOWN:
              for (i = (int) player.x; i < (int) player.x + player.w; i++)
                for (j = (int) player.y + player.h;
                     j <= (int) player.y + player.h + (int) player.dy + 2;
                     j++)
                  if (i >= ennemy[n].x && i <= ennemy[n].x + ennemy[n].w
                      && j >= ennemy[n].y && j <= ennemy[n].y + ennemy[n].h)
                    return 1;
              break;
            case EVENT_UP:
              for (i = (int) player.x; i < (int) player.x + player.w; i++)
                for (j = (int) player.y;
                     j >= (int) player.y - abs ((int) player.dy) - 1; j--)
                  if (i >= ennemy[n].x && i <= ennemy[n].x + ennemy[n].w
                      && j >= ennemy[n].y && j <= ennemy[n].y + ennemy[n].h)
                    return 1;
              break;
            default:
              return 1;
              break;
            }
        }
    }

  return 0;

}

int
ennemyCollisions (int cote)
{
  int n;

  for (n = 0; n < 10; n++)
    {
      if (ennemy[n].x != 0)
        {
          if (testCollisions
              (ennemy[n].x - 2, ennemy[n].y - 1, ennemy[n].w + 4, ennemy[n].h,
               (int) player.x, (int) player.y, player.w, player.h))
            switch (cote)
              {
              case EVENT_LEFT:
                if (ennemy[n].direction == 0 && ennemy[n].sens == 1)
                  return 1;
                break;
              case EVENT_RIGHT:
                if (ennemy[n].sens == -1 && ennemy[n].direction == 0)
                  return 1;
                break;
              case EVENT_DOWN:
                if (ennemy[n].sens == -1 && ennemy[n].direction == 1)
                  return 1;
                break;
              case EVENT_UP:
                if (ennemy[n].sens == 1 && ennemy[n].direction == 1)
                  return 1;
                break;
              default:
                return 1;
                break;
              }
        }
    }

  return 0;
}

int
testRopeCollision ()
{
  float i, j, k, dx, dy;
  float hyp;                    //int debx, deby, finx, finy;


  hyp =
    (player.x + player.w / 2 - XCable) * (player.x + player.w / 2 - XCable) +
    (player.y - YCable) * (player.y - YCable);
  hyp = sqrt (hyp);
  dx = (player.x + player.w / 2 - XCable) / hyp;
  dy = (player.y - YCable) / hyp;

  for (i = 0, j = XCable, k = YCable; i < hyp; i++, j += dx, k += dy)
    {
      if (collision ((int) j, (int) k))
        return 1;
    }

  return 0;
}

/*! this function tests if a shoot is in collision
* It returns 1 if there is a collision with the game board
* It returns 2 if there is a collision with the player
* It returns 0 if there is no collision
* \param x the abscissa of the shoot
* \param y the ordonnee of the shoot 
*/
int
testShootCollisions (int x, int y)
{
  int i, j;

  for (i = (x) / width * width; i <= (x + FireW) / width * width; i += width)
    {
      for (j = (y) / height * height; j <= (y + FireH) / height * height;
           j += height)
        {
          if (collision (i, j) || i > X || i < 0 || j > line * height
              || j < 0)
            {
              return 1;
            }
        }
    }

  if (testCollisions
      (x, y, FireW, FireH, (int) player.x, (int) player.y, player.w,
       player.h))
    {
      if (player.undying == 0)
        {
          return 2;
        }
      else
        {
          return 1;
        }
    }

  return 0;
}

int
detectCollision (SDL_Surface * sprite1, int x1, int y1, SDL_Surface * sprite2,
                 int x2, int y2)
{
  int rect1_x, rect1_y;
  int rect2_x, rect2_y;
  int i, j, k, l;
  int mask1, mask2;

  //Détection par bounding box
  //Retourne 0 et sort de la fonction 
  //si les sprites ne possédent pas de zones superposées
  if (x1 > x2 + sprite2->w)
    return 0;
  if (x1 + sprite1->w < x2)
    return 0;
  if (y1 > y2 + sprite2->h)
    return 0;
  if (y1 + sprite1->h < y2)
    return 0;

//      return 1;

  //Le but des lignes suivantes est de définir un 
  //rectangle qui englobe la zone d'affichage 
  //commune aux deux sprites
  //On traite les coordonnées x du rectangle
  if (x1 < x2)
    {
      rect1_x = x2;
      if (x1 + sprite1->w >= x2 + sprite2->w)
        rect2_x = x2 + sprite2->w;
      else
        rect2_x = x1 + sprite1->w;
    }
  else
    {
      rect1_x = x1;
      if (x2 + sprite2->w >= x1 + sprite1->w)
        rect2_x = x1 + sprite1->w;
      else
        rect2_x = x2 + sprite2->w;
    }

  //On traite les coordonnées y du rectangle
  if (y1 < y2)
    {
      rect1_y = y2;
      if (y1 + sprite1->h >= y2 + sprite2->h)
        rect2_y = y2 + sprite2->h;
      else
        rect2_y = y1 + sprite1->h;
    }
  else
    {
      rect1_y = y1;
      if (y2 + sprite2->h > y1 + sprite1->h)
        rect2_y = y1 + sprite1->h;
      else
        rect2_y = y2 + sprite2->h;
    }

  //Il ne reste plus qu'à tester pour chaque 
  //pixel du rectangle précèdemment défini si ses pixels 
  //sont transparents
  //Un pixel non transparent signifie qu'un bout de sprite 
  //est present dans le rectangle
  //et donc que les sprites sont en collision
  mask1 = sprite1->format->colorkey;
  mask2 = sprite2->format->colorkey;

  for (i = rect1_x - x1, j = rect1_x - x2; i < rect2_x - x1; i++, j++)
    {
      for (k = rect1_y - y1, l = rect1_y - y2; k < rect2_y - y1; k++, l++)
        {
          if ((int) getpixel (sprite1, i, k) != mask1
              && (int) getpixel (sprite2, j, l) != mask2)
            return 1;
        }
    }
  return 0;
}
