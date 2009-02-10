/**
 * @file rope.c
 * @brief 
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
#ifdef WIN32
#include <io.h>
#endif
float array[101][27];
float ropew;
float a, b;

/**************************************************
                    FUNCTIONS
**************************************************/

void
loadRope ()
{
  SDL_RWops *op;
  op = TLK_RWFromZip (PAKNAME, "rope.dat");
  if (op != 0)
    {
      SDL_RWread (op, &array, sizeof (unsigned char), 101 * 27 * 4);
      op->close (op);
    }
  else
    {
      fprintf (stderr, "Error while trying to load %s/%s\n", PAKNAME,
               "rope.dat");
      exit (1);
    }

  /*handle=loadFileFromZip(PAKNAME, "ROPE.DAT"); fopen("DATAS/ROPE.DAT", "rb");

     if (handle!=NULL)
     {        
     fread(array, sizeof(int), 101*27*4, handle);
     fclose(handle);
     } */
}

void
findAB (float l, float xo, float yo)
{
  float dx, dy;
  float bx, by;
  float m1, m2;

  bx = (xo / l) * 50 + 50;
  by = (yo / l) * 13 + 13;

  dx = bx - (float) ((int) bx);
  dy = by - (float) ((int) by);

  m1 =
    array[(int) bx][(int) by] * (1 - dy) + array[(int) bx][(int) by + 1] * dy;
  m2 =
    array[(int) bx + 1][(int) by] * (1 - dy) + array[(int) bx + 1][(int) by +
                                                                   1] * dy;

  a = m1 * (1 - dx) + m2 * dx;
  a = a / l;

  b = yo / xo - a * xo;
}

void
moveRope ()
{
  float a, b;
  float k, l, l2;

  a = XCable - (player.x + (float) player.w / 2);
  b = YCable - (player.y + (float) player.h / 2);

  l2 = fabs (a * a + b * b);

  /* Hypothenuse */
  l = sqrt (l2);                /* Longueur avec ancien deplacement */

/* fprintf(stdout, "ropew : %f, l :%f , x : %d , y : %d\n", ropew, l, (int)player.x, (int)player.y); */

  if (l != ropew)               /* Si hors du cercle */
    {
      player.x = XCable - (a * ropew / l) - player.w / 2;       /* Coordonnees bord */

      if (testDownCollisions ())        /* manageCollisions(0, EVENT_DOWN, (int)player.x, (int)(YCable-(b*ropew/l))+player.h, player.w, 1)) */
        {
          a = XCable - (player.x + (float) player.w / 2);
          b = YCable - (player.y + (float) player.h / 2);

          l2 = fabs (a * a + b * b);

          /* Hypothenuse */
          ropew = sqrt (l2);
        }
      else
        {
          player.y = YCable - (b * ropew / l) - player.h / 2;

          k = (a * player.dy - b * player.dx) / l2;
          player.dx = -2.01 * b * k - player.dx;        /* Vitesse rebond */
          player.dy = 2.01 * a * k - player.dy;
        }
    }

/* fprintf(stdout, "new l : %f , x : %d , y : %d\n", l, (int)player.x, (int)player.y); */

}

int
displayRope ()
/*{
    displayLine(screen, (int)XCable, (int)YCable, (int)player.x+player.w/2, (int)player.y, 200, 200, 200);
    return 1;
} */
{
  int i;
  int ox, oy;
  float x, y, Ptx;

  Ptx = XCable - (player.x + player.w / 2);
  if (fabs (Ptx) < 3)
    {
      if (ropew > player.y + player.h / 2 - YCable)
        /* if (ropew>YCable-player.y+2) */
        {
          {
            displayLine (screen, (int) XCable, (int) YCable, (int) XCable,
                         (int) (ropew + YCable + player.y + player.h / 2) / 2,
                         200, 200, 200);
            displayLine (screen, (int) XCable,
                         (int) (ropew + YCable + player.y + player.h / 2) / 2,
                         (int) player.x + player.w / 2,
                         (int) player.y + player.h / 2, 200, 200, 200);
            /* TRACED(XCable,YCable,XCable,(int)(ropew+YCable+player.y)/2,13,0); */
            /* TRACED(XCable,(int)(ropew+YCable+player.y)/2,(int)(player.x),(int)(player.y),13,0); */
          }

        }
      else
        {
          displayLine (screen, (int) XCable, (int) YCable,
                       (int) (player.x) + player.w / 2,
                       (int) (player.y + player.h / 2), 200, 200, 200);
        }
      return (0);
    }

  findAB (ropew, Ptx, YCable - (player.y + player.h / 2));
  ox = (int) player.x + player.w / 2;
  oy = (int) player.y + player.h / 2;
  for (i = 1; i < 11; i++)
    {
      x = Ptx * ((float) i) / 10;
      y = x * x * a + x * b + player.y + player.h / 2;
      x += player.x + player.w / 2;

      if ((ox != ((int) x)) || (oy != ((int) y)))
        displayLine (screen, ox, oy, (int) x, (int) y, 200, 200, 200);

      ox = (int) x;
      oy = (int) y;
    }

  return 1;
}
