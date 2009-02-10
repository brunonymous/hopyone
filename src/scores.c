/**
 * @file scores.c 
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
Sscore HiScore[10];

void
save (int nb)
{
  FILE *file;
  int i;
  char buffer[100];

  sprintf (buffer, "%s%d.sav", REP_SAVE "hopy", nb);
  file = fopen (buffer, "wb");

  fprintf (file, "%d ", player.level);
  fprintf (file, "%d ", player.ammo);
  fprintf (file, "%d ", player.life);
  fprintf (file, "%d ", player.score);

  for (i = 0; i < 100; i++)
    fprintf (file, "%d ", Bonus[i]);

  fclose (file);
}

void
load (int nb)
{
  FILE *file;
  int i;
  char buffer[100];

  sprintf (buffer, "%s%d.sav", REP_SAVE "hopy", nb);
  file = fopen (buffer, "rb");

  fscanf (file, "%d", &player.level);
  fscanf (file, "%d", &player.ammo);
  fscanf (file, "%d", &player.life);
  fscanf (file, "%d", &player.score);

  for (i = 0; i < 100; i++)
    fscanf (file, "%d", &Bonus[i]);

  fclose (file);
}

void
loadHighScores (void)
{
  FILE *file;

  int i;

  file = fopen (REP_SAVE "hiscores.dat", "rb");
  if (file == NULL)
    {
      fprintf(stderr, "scores.c/loadHighScores()"
              "fopen() failed");
      return;
    }

  for (i = 0; i < 10; i++)
    {
      fscanf (file, "%d %s", &HiScore[i].score, (char*)&HiScore[i].name);
    }

  fclose (file);
}

void
saveHighScores ()
{
  FILE *file;

  int i;

  file = fopen (REP_SAVE "hiscores.dat", "wb");

  for (i = 0; i < 10; i++)
    fprintf (file, "%d %s\n", HiScore[i].score, HiScore[i].name);

  fclose (file);
}

void
resetHighScores ()
{
  int i;

  for (i = 0; i < 10; i++)
    {
      HiScore[i].score = (10 - i) * 1000;
      sprintf (HiScore[i].name, "TLK");
    }
}
