/**
 * @file burst.c 
 * @brief 
 * @created 2007-12-13 
 * @date 2009-02-19
 * @author Gregory Cauchois
 * @author Bruno Ethvignot
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
static Sint32 nbStars = 0;

void
manageBursts (void)
{
  Sint32 i;

  for (i = 0; i < NB_BURSTS; i++)
    {
      if (burst[i].life >= 0)
        {
          burst[i].life--;
        }
    }
  for (i = 0; i < NB_BIGBURSTS; i++)
    {
      if (bigburst[i].life >= 0)
        {
          bigburst[i].life--;
        }
    }
}

void
displayBursts (void)
{
  Sint32 i;

  for (i = 0; i < NB_BURSTS; i++)
    {
      if (burst[i].life > 0)
        {
          copyPicture (IMG_spoing, screen,
                       775 + burst[i].life / (LIFE_BURST / 8) * 25, 0, 25, 32,
                       burst[i].x - (i % 2 == 1 ? 8 : 0), burst[i].y - 16);
        }
    }
  for (i = 0; i < NB_BIGBURSTS; i++)
    {
      if (bigburst[i].life > 0)
        {
          /* - (i%2==1 ? 8 : 0) */
          copyPicture (IMG_spoing, screen, 550 + bigburst[i].life / (LIFE_BIGBURST / 8) * 50, 255, 50, 64, bigburst[i].x, bigburst[i].y);
        }
    }
}

void
addBurst (Sint32 x, Sint32 y)
{
  Sint32 i;

  i = 0;
  while (burst[i].life > 0 && i < NB_BURSTS)
    {
      i++;
    }

  if (i < NB_BURSTS)
    {
      burst[i].x = x;
      burst[i].y = y;
      burst[i].life = LIFE_BURST;
      playSound (3);
    }
}

void
addBigBurst (Sint32 x, Sint32 y)
{
  Sint32 i;

  i = 0;
  while (bigburst[i].life > 0 && i < NB_BIGBURSTS)
    {
      i++;
    }

  if (i < NB_BIGBURSTS)
    {
      bigburst[i].x = x;
      bigburst[i].y = y;
      bigburst[i].life = LIFE_BIGBURST;
      playSound (2);
    }
}

void
initStar (void)
{
  Uint32 i;

  for (i = 0; i < NB_STARS; i++)
    {
      stars[i].life = -1;
    }
  nbStars = 0;
}

void
addStars (void)
{
  nbStars = STARS;
}

void
manageStars (void)
{
  static Sint32 tps = 0;
  Sint32 i;

  tps++;

  if (tps == 5)
    {
      tps = 0;

      if (nbStars > 0)
        {
          addStar ((Sint32) player.x - player.w / 2 + rand () % (player.w * 2),
                   (Sint32) player.y - player.h / 2 + rand () % (player.h * 2));
        }
    }

  for (i = 0; i < NB_STARS; i++)
    {
      if (stars[i].life != -1)
        {
          stars[i].life--;
        }
    }
}

void
addStar (Sint32 x, Sint32 y)
{
  Uint32 i;
  i = 0;

  while (stars[i].life != -1 && i < NB_STARS)
    {
      i++;
    }

  if (i < NB_STARS)
    {
      stars[i].x = x;
      stars[i].y = y;
      stars[i].life = LIFE_STAR;
      nbStars--;
    }
}

void
displayStar (void)
{
  Uint32 i;

  for (i = 0; i < NB_STARS; i++)
    {
      if (stars[i].life != -1)
        {
          copyPicture (IMG_spoing, screen,
                       250 + (13 * stars[i].life / LIFE_STAR * 8), 304, 13,
                       16, stars[i].x, stars[i].y);
        }
    }
}

void
displayCollapsingBlocks (void)
{
  Sint32 i;

  for (i = 0; i < NB_COLLAPSINGBLOCK; i++)
    {
      if (collapsingBlock[i].life > 0)
        {
          copyPicture (IMG_map, IMG_decor,
                       450 +
                       (5 * (LIFE_COLLAPSINGBLOCK - collapsingBlock[i].life) /
                        LIFE_COLLAPSINGBLOCK) * width, 0, width, height,
                       collapsingBlock[i].x, collapsingBlock[i].y);
        }
    }

}

void
addCollapsingBlock (Sint32 x, Sint32 y)
{
  Sint32 i;

  i = 0;
  while (collapsingBlock[i].life > 0 && i < NB_COLLAPSINGBLOCK)
    {
      i++;
    }

  if (i < NB_COLLAPSINGBLOCK)
    {
      collapsingBlock[i].x = x;
      collapsingBlock[i].y = y;
      collapsingBlock[i].life = LIFE_COLLAPSINGBLOCK;
    }
}

void
loadCollapsingBlocks (void)
{
  Sint32 i, j;

  for (i = 0; i < NB_COLLAPSINGBLOCK; i++)
    {
      collapsingBlock[i].life = -1;
    }

  for (i = 0; i < column; i++)
    {
      for (j = 0; j < line; j++)
        {
          if (Col[j * column + i] == COL_EFFRITE)
            {
              addCollapsingBlock (i * width, j * height);
            }
        }
    }
}

void
collapseCollapsingBlock (Sint32 x, Sint32 y)
{
  Sint32 i;

  for (i = 0; i < NB_COLLAPSINGBLOCK; i++)
    {
      if (collapsingBlock[i].life > 0)
        {
          if (collapsingBlock[i].x == x && collapsingBlock[i].y == y)
            {
              collapsingBlock[i].life--;
              if (collapsingBlock[i].life == 0)
                {
                  Col[collapsingBlock[i].y / height * column +
                      collapsingBlock[i].x / width] = COL_VIDE;
                  displayShadow (IMG_decor, collapsingBlock[i].x / width,
                                 collapsingBlock[i].y / height);
                }
            }
        }
    }
}

void
manageCollapsingBlocks (void)
{
  Sint32 i, j;

  for (i =
       ((Sint32) player.x - 2 +
        (player.dx < 0 ? (Sint32) player.dx : 0)) / width * width;
       i <=
       ((Sint32) player.x + player.w + 2 +
        (player.dx > 0 ? (Sint32) player.dx : 0)) / width * width; i += width)
    {
      for (j =
           ((Sint32) player.y - 2 +
            (player.dy < 0 ? (Sint32) player.dy : 0)) / height * height;
           j <=
           ((Sint32) player.y + player.h + 2 +
            (player.dy > 0 ? (Sint32) player.dy : 0)) / height * height;
           j += height)
        {
          if (getCollision (i, j) == COL_EFFRITE)
            {
              collapseCollapsingBlock (i, j);
            }
        }
    }


  for (i = 0; i < NB_COLLAPSINGBLOCK; i++)
    {
      if (collapsingBlock[i].life < LIFE_COLLAPSINGBLOCK)
        {
          collapseCollapsingBlock (collapsingBlock[i].x,
                                   collapsingBlock[i].y);
        }
    }
}
