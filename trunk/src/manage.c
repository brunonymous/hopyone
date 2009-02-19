/**
 * @file manage.c 
 * @brief 
 * @created 2007-11-12 
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
int delta_time = 16;

/**************************************************
                    FUNCTIONS
**************************************************/

void
displayCursor (void)
{
  copyPicture (IMG_mouse, screen, 0, 0, IMG_mouse->w, IMG_mouse->h,
               mouse.motionX, mouse.motionY);
  /* displayFullSquare(surface, mouse.motionX, mouse.motionY, 10, 10, 200, 200, 200); */
}

void
manageConfiguration ()
{
  int done = 0, i;
  char buffer[200];
  int indice = 0;
  int ordonnee = 150;

  char conf_name[10][100];
  char conf_option[10][64];
  printf("manageConfiguration\n");

  sprintf (conf_name[0], "%s", text[TEXT_LANG]);
  sprintf (conf_option[0], "%s", cfg_lang);
  sprintf (conf_name[1], "%s", text[TEXT_TEXT]);
  sprintf (conf_option[1], "%s", cfg_text);
  sprintf (conf_name[2], "%s", text[TEXT_ANIMATION]);
  sprintf (conf_option[2], "%s", cfg_animation);
  sprintf (conf_name[3], "%s", text[TEXT_MUSIC]);
  sprintf (conf_option[3], "%s", cfg_music);
  sprintf (conf_name[4], "%s", text[TEXT_SOUND]);
  sprintf (conf_option[4], "%s", cfg_sound);
  sprintf (conf_name[5], "%s", text[TEXT_MUSICVOLUME]);
  sprintf (conf_option[5], "%d", musicvol);
  sprintf (conf_name[6], "%s", text[TEXT_SOUNDVOLUME]);
  sprintf (conf_option[6], "%d", soundvol);

  do
    {
      copyPicture (fond_menu, screen, 0, 0, fond_menu->w, fond_menu->h, 0, 0);

      for (i = 0; i < 7; i++)
        {
          sprintf (buffer, "%s : %s", conf_name[i], conf_option[i]);

          if (indice == i)
            printTextShaded (screen, TTFfontmenu2, buffer,
                             centerText (TTFfontmenu2, buffer, 0, X),
                             ordonnee + i * TTF_FontLineSkip (TTFfontmenu2),
                             200, 50, 50);
          else
            printTextShaded (screen, TTFfontmenu2, buffer,
                             centerText (TTFfontmenu2, buffer, 0, X),
                             ordonnee + i * TTF_FontLineSkip (TTFfontmenu2),
                             100, 100, 100);
        }

      SDL_Flip (screen);

      manageKeys ();

      if (event[EVENT_PRESSEDDOWN])
        indice = (indice + 1) % 7;
      if (event[EVENT_PRESSEDUP])
        indice = (indice + 6) % 7;

      if (event[EVENT_PRESSEDLEFT] || event[EVENT_PRESSEDRIGHT])
        {
          switch (indice)
            {
            case 0:
              sprintf (conf_option[0], "%s",
                       strcmp (conf_option[0],
                               "french") == 0 ? "english" : "french");
              break;
            case 1:
              sprintf (conf_option[indice], "%s",
                       strcmp (conf_option[indice],
                               "blended") == 0 ? "normal" : "blended");
              break;
            case 2:
            case 3:
            case 4:
              sprintf (conf_option[indice], "%s",
                       strcmp (conf_option[indice],
                               "on") == 0 ? "off" : "on");
              break;
            case 5:
            case 6:
              if (event[EVENT_PRESSEDRIGHT]
                  && atoi (conf_option[indice]) < MIX_MAX_VOLUME)
                sprintf (conf_option[indice], "%d",
                         atoi (conf_option[indice]) + 1);
              if (event[EVENT_PRESSEDLEFT] && atoi (conf_option[indice]) > 0)
                sprintf (conf_option[indice], "%d",
                         atoi (conf_option[indice]) - 1);
              break;
            }
        }

      if (event[EVENT_ENTER])
        {
          done = 1;

          sprintf (cfg_lang, "%s", conf_option[0]);
          loadText ();
          sprintf (cfg_text, "%s", conf_option[1]);
          sprintf (cfg_animation, "%s", conf_option[2]);
          sprintf (cfg_music, "%s", conf_option[3]);
          sprintf (cfg_sound, "%s", conf_option[4]);
          musicvol = atoi (conf_option[5]);
          soundvol = atoi (conf_option[6]);

          saveINI ();
        }

      if (event[EVENT_QUIT])
        {
          done = 1;
          event[EVENT_QUIT] = 0;
        }

    }
  while (!done);

}

void
manageHiScores (int update)
{
  int i, j;
  char buffer[200];
  SDL_Event event_list;

  if (update)
    {
      j = 0;
      while (HiScore[j].score > player.score)
        j++;

      for (i = 9; i > j; i--)
        {
          HiScore[i].score = HiScore[i - 1].score;
          sprintf (HiScore[i].name, "%s", HiScore[i - 1].name);
        }

      HiScore[j].score = player.score;
      sprintf (HiScore[i].name, "       ");

      int done = 0;
      i = 0;
      while (!done)
        {
          copyPicture (gameover, screen, 0, 0, gameover->w, gameover->h, 0,
                       0);
          printTextShaded (screen, TTFfontmenu2, text[TEXT_HISCORE],
                           centerText (TTFfontmenu2, text[TEXT_HISCORE], 0,
                                       X), 100, 0, 100, 0);
          printTextShaded (screen, TTFfontmenu2, text[TEXT_ENTERNAME],
                           centerText (TTFfontmenu2, text[TEXT_ENTERNAME], 0,
                                       X), 200, 0, 100, 100);
          printTextShaded (screen, TTFfontmenu2, HiScore[j].name,
                           centerText (TTFfontmenu2, HiScore[j].name, 0, X),
                           300, 0, 100, 200);
          SDL_Flip (screen);

          /* Gestion des événements */
          while (SDL_PollEvent (&event_list))
            {
              switch (event_list.type)
                {
                  /* si touche enfoncée */
                case SDL_KEYDOWN:
                  switch (event_list.key.keysym.unicode)
                    {
                    case SDLK_RETURN:
                      done = 1;
                      break;

                    case SDLK_BACKSPACE:
                      if (i > 0)
                        HiScore[j].name[--i] = ' ';
                      break;

                    default:
                      if (i < 8 && event_list.key.keysym.sym < 127)
                        HiScore[j].name[i++] = event_list.key.keysym.unicode;
                      break;
                    }
                  break;
                }
            }
        }

      saveHighScores ();
    }

  do
    {

      copyPicture (gameover, screen, 0, 0, gameover->w, gameover->h, 0, 0);

      for (i = 0; i < 10; i++)
        {
          char buffer2[8];

          sprintf (buffer2, "%s", HiScore[i].name);
          /*
             for (j=strlen(HiScore[i].name);j<8;j++)
             buffer2[j]=' ';
             buffer2[j]='\0';
           */
          sprintf (buffer, "%.2d. %s :", i + 1, buffer2);
          printTextShaded (screen, TTFfontmenu2, buffer, X / 3,
                           100 + i * TTF_FontLineSkip (TTFfontmenu2), 80, 80,
                           255);
          sprintf (buffer, "%.8d", HiScore[i].score);
          printTextShaded (screen, TTFfontmenu2, buffer, 380,
                           100 + i * TTF_FontLineSkip (TTFfontmenu2), 80, 80,
                           255);
        }

      SDL_Flip (screen);
    }
  while (!keyPressed ());

}

void
manageMenu ()
{
  int done = 0, i, decalage = 0, dx = 1;
  int indice_menu = 0, state;
  int keydelay = 0;

  /*
     sprintf(filename, "%s/Menu.jp2", REP_GFX);
     menu=loadPicture(filename);
     menu=createMenu();
   */

  while (!done)
    {
      NewTick = SDL_GetTicks ();

      /* clear screen */
      clearSurface (screen);

      copyPicture (fond_menu, screen, 0, 0, X, Y, 0, 0);

      int abscisse = 0, ordonnee = 150, taille = X;
      int r = 100, g = 100, b = 100;
      int r2 = 200, g2 = 50, b2 = 50;

      decalage += dx;
      if (decalage >= 4 || decalage <= -4)
        dx = -dx;

      for (i = 0; i < 9; i++)
        {
          if (i != indice_menu)
            {
              printTextBlended (screen, TTFfontmenu, text[TEXT_START + i],
                                centerText (TTFfontmenu, text[TEXT_START + i],
                                            abscisse, abscisse + taille),
                                ordonnee + i * TTF_FontLineSkip (TTFfontmenu),
                                r - 50, g - 50, b - 50);
              printTextBlended (screen, TTFfontmenu, text[TEXT_START + i],
                                centerText (TTFfontmenu, text[TEXT_START + i],
                                            abscisse, abscisse + taille - 5),
                                ordonnee +
                                i * TTF_FontLineSkip (TTFfontmenu) - 1, r, g,
                                b);
            }
          else
            {
              printTextBlended (screen, TTFfontmenu2, text[TEXT_START + i],
                                centerText (TTFfontmenu2,
                                            text[TEXT_START + i],
                                            abscisse + decalage,
                                            abscisse + decalage + taille),
                                ordonnee + i * TTF_FontLineSkip (TTFfontmenu),
                                r2 - 50, g2 - 50, b2 - 50);
              printTextBlended (screen, TTFfontmenu2, text[TEXT_START + i],
                                centerText (TTFfontmenu2,
                                            text[TEXT_START + i],
                                            abscisse + decalage,
                                            abscisse + decalage + taille - 5),
                                ordonnee +
                                i * TTF_FontLineSkip (TTFfontmenu) - 1, r2,
                                g2, b2);
            }
        }

      displayCursor ();

      SDL_Flip (screen);

      manageKeys ();

      if (event[EVENT_DOWN] || event[EVENT_UP])
        keydelay++;

      if (event[EVENT_PRESSEDDOWN] || (event[EVENT_DOWN] && keydelay > 10))
        {
          keydelay = 0;
          indice_menu = (indice_menu + 1) % 9;
        }

      if (event[EVENT_PRESSEDUP] || (event[EVENT_UP] && keydelay > 10))
        {
          keydelay = 0;
          indice_menu = (indice_menu + 8) % 9;
        }

      if (event[EVENT_MOTION])
        {
          indice_menu =
            (mouse.motionY - ordonnee) / TTF_FontLineSkip (TTFfontmenu);
          if (indice_menu < 0)
            indice_menu = 0;
          if (indice_menu > 8)
            indice_menu = 8;
        }

      if (event[EVENT_ENTER] || event[EVENT_CLIC])
        {
          switch (indice_menu)
            {
            case 0:
              state = manageGame ();
              if (state == 2)
                {
                  printTextShaded (gameover, TTFfont, text[TEXT_GAMEOVER],
                                   centerText (TTFfont, text[TEXT_GAMEOVER],
                                               0, X), Y / 2, 100, 100, 100);

                  animate (SDL_GetVideoSurface (), gameover,
                           EVENT_LEFT + rand () % 4);

                  /* SDL_Delay(1000); */

                  while (!keyPressed ());

                  if (player.score > HiScore[9].score)
                    manageHiScores (1);

                  resetGame ();
                  /* initGame(); */
                  initMap ();

                }

              break;

            case 2:
              manageSaveLoad ();
              break;

            case 3:
              manageConfiguration ();
              break;

            case 6:
              //displayPage(3,0);
              break;

            case 7:
              manageHiScores (0);
              break;

            case 8:
              done = 1;
              break;
            }
        }

      if (event[EVENT_FULLSCREEN])
        {
          fullscreen = !fullscreen;
          SDL_FreeSurface (screen);
          if ((screen =
               SDL_SetVideoMode (X, Y, COLOR_DEPTH,
                                 SDL_SWSURFACE | SDL_DOUBLEBUF | (fullscreen ?
                                                                  SDL_FULLSCREEN
                                                                  : 0))) ==
              NULL)
            {
              fprintf (stderr, "Erreur lors du passage en %s",
                       (fullscreen ? "fullscreen" : "mode fenetre"));
            }
        }

      if (event[EVENT_QUIT])
        done = 1;

      /* SDL_Delay(80); */
    }

}

void
manageSaveLoad ()
{
  char buffer[100];
  int indice_menu = 0, indice_menu2 = 0;
  int done = 0, i, ordonnee = 150;
  int r = 100, g = 100, b = 100;
  int r2 = 200, g2 = 50, b2 = 50;
  int compteur = 0, dx = 1;
  int level[3];
  FILE *file;
  int keydelay = 0;

  for (i = 0; i < 3; i++)
    {
      sprintf (buffer, "%s%d.sav", REP_SAVE "hopy", i);
      file = fopen (buffer, "rb");
      fscanf (file, "%d", &level[i]);
      fclose (file);
    }

  while (!done)
    {
      compteur += dx;
      if (compteur >= 9 || compteur <= 0)
        dx = -dx;

      clearSurface (screen);

      for (i = 0; i < 100; i++)
        event[i] = 0;

      copyPicture (fond_menu, screen, 0, 0, X, Y, 0, 0);

      sprintf (buffer, "%s", text[TEXT_SELECTFILE]);
      printTextBlended (screen, TTFfontmenu2, buffer,
                        centerText (TTFfontmenu2, buffer, 0, X),
                        50 + TTF_FontLineSkip (TTFfontmenu), r2 - 50, g2 - 50,
                        b2 - 50);
      printTextBlended (screen, TTFfontmenu2, buffer,
                        centerText (TTFfontmenu2, buffer, -3, X - 3),
                        50 + TTF_FontLineSkip (TTFfontmenu) - 1, r2, g2, b2);

      ordonnee = 150;

      for (i = 0; i < 3; i++)
        {
          sprintf (buffer, "%s %d (%s %d)", text[TEXT_FILENAME], i + 1,
                   text[TEXT_LEVEL], level[i]);

          if (i == indice_menu)
            {
              printTextBlended (screen, TTFfontmenu2, buffer,
                                centerText (TTFfontmenu2, buffer,
                                            5 - compteur, 5 - compteur + X),
                                ordonnee + i * TTF_FontLineSkip (TTFfontmenu),
                                r2 - 50, g2 - 50, b2 - 50);
              printTextBlended (screen, TTFfontmenu2, buffer,
                                centerText (TTFfontmenu2, buffer,
                                            5 - compteur,
                                            5 - compteur + X - 5),
                                ordonnee +
                                i * TTF_FontLineSkip (TTFfontmenu) - 1, r2,
                                g2, b2);
            }
          else
            {
              printTextBlended (screen, TTFfontmenu, buffer,
                                centerText (TTFfontmenu, buffer, 0, X),
                                ordonnee + i * TTF_FontLineSkip (TTFfontmenu),
                                r - 50, g - 50, b - 50);
              printTextBlended (screen, TTFfontmenu, buffer,
                                centerText (TTFfontmenu, buffer, 0, X - 5),
                                ordonnee +
                                i * TTF_FontLineSkip (TTFfontmenu) - 1, r, g,
                                b);
            }
        }

      ordonnee = 300;

      for (i = 0; i < 2; i++)
        {
          sprintf (buffer, "%s", text[TEXT_SAVE + i]);

          if (i == indice_menu2)
            {
              printTextBlended (screen, TTFfontmenu2, buffer,
                                centerText (TTFfontmenu2, buffer, 0, X),
                                ordonnee + i * TTF_FontLineSkip (TTFfontmenu),
                                r2 - 50, g2 - 50, b2 - 50);
              printTextBlended (screen, TTFfontmenu2, buffer,
                                centerText (TTFfontmenu2, buffer, 0, X - 5),
                                ordonnee +
                                i * TTF_FontLineSkip (TTFfontmenu) - 1, r2,
                                g2, b2);
            }
          else
            {
              printTextBlended (screen, TTFfontmenu, buffer,
                                centerText (TTFfontmenu, buffer, 0, X),
                                ordonnee + i * TTF_FontLineSkip (TTFfontmenu),
                                r - 50, g - 50, b - 50);
              printTextBlended (screen, TTFfontmenu, buffer,
                                centerText (TTFfontmenu, buffer, 0, X - 5),
                                ordonnee +
                                i * TTF_FontLineSkip (TTFfontmenu) - 1, r, g,
                                b);
            }


        }

      SDL_Flip (screen);

      manageKeys ();

      if (event[EVENT_DOWN] || event[EVENT_UP] || event[EVENT_RIGHT]
          || event[EVENT_LEFT])
        keydelay++;

      if (event[EVENT_PRESSEDDOWN] || (event[EVENT_DOWN] && keydelay > 10))
        {
          keydelay = 0;
          indice_menu = (indice_menu + 1) % 3;
        }

      if (event[EVENT_PRESSEDUP] || (event[EVENT_UP] && keydelay > 10))
        {
          keydelay = 0;
          indice_menu = (indice_menu + 2) % 3;
        }

      if (event[EVENT_PRESSEDRIGHT] || (event[EVENT_RIGHT] && keydelay > 10))
        {
          keydelay = 0;
          indice_menu2 = (indice_menu2 + 1) % 2;
        }

      if (event[EVENT_PRESSEDLEFT] || (event[EVENT_LEFT] && keydelay > 10))
        {
          keydelay = 0;
          indice_menu2 = (indice_menu2 + 1) % 2;
        }

      if (event[EVENT_QUIT])
        {
          done = 1;
          event[EVENT_QUIT] = 0;
        }

      if (event[EVENT_ENTER])
        {
          if (indice_menu2 == 0)
            {
              save (indice_menu);
            }
          else
            {
              load (indice_menu);
              initMap ();
            }

          done = 1;
        }

      /* SDL_Delay(120); */
    }
}

void
manageShoots ()
{
  int i, j, n;
  static int tempo;
  int FireSpeed;

  FireSpeed = (int) ((float) FIRESPEED * (float) delta_time / (float) 1024);


  for (n = 0; n < 20; n += 2)
    {
      if (playerShoot[n].state)
        {
          for (i = playerShoot[n].x / width * width;
               i <= (playerShoot[n].x + FireW + FireSpeed) / width * width;
               i += width)
            for (j = (playerShoot[n].y + 1) / height * height;
                 j <= (playerShoot[n].y + FireH) / height * height;
                 j += height)
              if (collision (i, j) || i > X)
                {
                  playerShoot[n].state = 0;
                  /* playSound(2); */
                }
          if (playerShoot[n].state)
            playerShoot[n].x -= FireSpeed;
          else
            addBurst (playerShoot[n].x, playerShoot[n].y);
        }

      if (playerShoot[n + 1].state)
        {
          for (i = (playerShoot[n + 1].x + FireW) / width * width;
               i >= (playerShoot[n + 1].x - FireSpeed) / width * width;
               i -= width)
            for (j = (playerShoot[n + 1].y + 1) / height * height;
                 j <= (playerShoot[n + 1].y + FireH) / height * height;
                 j += height)
              if (collision (i, j) || i < 0)
                {
                  playerShoot[n + 1].state = 0;
                  /* playSound(2); */
                }
          if (playerShoot[n + 1].state)
            playerShoot[n + 1].x += FireSpeed;
          else
            addBurst (playerShoot[n + 1].x, playerShoot[n + 1].y);
        }
    }

  for (i = 0; i < 10; i++)
    {
      if (detectingArea[i].x != 0)
        {
          if (tempo % 60 == 0)
            {
              if (testCollisions
                  ((int) player.x, (int) player.y, player.w, player.h,
                   detectingArea[i].debx, detectingArea[i].deby,
                   detectingArea[i].finx - detectingArea[i].debx,
                   detectingArea[i].finy - detectingArea[i].deby))
                {
                  /* fprintf(stdout, "detect !!!\n"); */
                  j = 0;
                  while (detectingShoot[j].state != 0 && j < NBSHOOTSE)
                    j++;
                  if (j < NBSHOOTSE)
                    {
                      detectingShoot[j].state = 1;
                      detectingShoot[j].x = detectingArea[i].x - width;
                      detectingShoot[j].y = detectingArea[i].y + height;
                      detectingShoot[j].dx =
                        (detectingArea[i].dx - detectingArea[i].x >
                         width ? FireSpeed / 2 : detectingArea[i].dx -
                         detectingArea[i].x == width ? 0 : -FireSpeed / 2);
                      detectingShoot[j].dy =
                        (detectingArea[i].dy - detectingArea[i].y >
                         height ? FireSpeed / 2 : detectingArea[i].dy -
                         detectingArea[i].y == height ? 0 : -FireSpeed / 2);
                    }
                }
            }
        }
    }

  tempo++;
  for (n = 0; n < NBSHOOTSE; n++)
    {
      if (detectingShoot[n].state)      /* && tempo%10==0) */
        {
          int state =
            testShootCollisions (detectingShoot[n].x +
                                 (int) detectingShoot[n].dx,
                                 detectingShoot[n].y +
                                 (int) detectingShoot[n].dy);

          switch (state)
            {
            case 0:
              detectingShoot[n].x += (int) detectingShoot[n].dx;
              detectingShoot[n].y += (int) detectingShoot[n].dy;
              break;
            case 1:
              detectingShoot[n].state = 0;
              addBurst (detectingShoot[n].x, detectingShoot[n].y);
              break;
            case 2:
              /* addBurst(detectingShoot[i].x, detectingShoot[i].y); */
              Die ();
              break;
            }
        }
    }
}

void
manageEnnemys ()
{
  int i, j, n;
  static int cadenceennemy = 0;

  /* enemies displacement */
  for (i = 0; i < 10; i++)
    {
      if (ennemy[i].x != 0)
        {

          /* heightennemy= ennemy[i].type==5 ? 10 : 23; */
          /* widthennemy= ennemy[i].type==5 ? 32 : 23; */

          if (ennemy[i].direction == 0)
            {
              if (manageCollisions
                  (0, ennemy[i].sens == 1 ? EVENT_RIGHT : EVENT_LEFT,
                   ennemy[i].x, ennemy[i].y,
                   ennemy[i].sens == 1 ? ennemy[i].w + 1 : -1, ennemy[i].h))
                {
                  ennemy[i].sens = -ennemy[i].sens;
                }
              else
                {
                  ennemy[i].x += ennemy[i].sens;
                }
            }
          if (ennemy[i].direction == 1)
            {
              if (manageCollisions
                  (0, ennemy[i].sens == 1 ? EVENT_DOWN : EVENT_UP,
                   ennemy[i].x, ennemy[i].y, ennemy[i].w,
                   ennemy[i].sens == 1 ? ennemy[i].h + 1 : -1))
                {
                  ennemy[i].sens = -ennemy[i].sens;
                }
              else
                {
                  ennemy[i].y += ennemy[i].sens;
                }
            }
        }
    }

  cadenceennemy++;
  /* 
     if (cadenceennemy==100)
     cadenceennemy=0; */

  /* cadence playerShoots */
  for (i = 0; i < 10; i++)
    {

      if (ennemy[i].x != 0 && ennemy[i].cadence_tir != 0)
        {

          if (cadenceennemy % (ennemy[i].cadence_tir * 20) == 0)
            {
              j = 0;
              while (ennemyShoot[j].state != 0 && j < NBSHOOTSE)
                j++;
              if (j < NBSHOOTSE)
                {
                  ennemyShoot[j].state = 1;
                  ennemyShoot[j].x = ennemy[i].x;
                  ennemyShoot[j].y = ennemy[i].y;
                  ennemyShoot[j].type = ennemy[i].type_tir;

                  switch (ennemyShoot[j].type)
                    {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                      ennemyShoot[j].dx =
                        ennemy[i].sens * (ennemyShoot[j].type + 1);
                      ennemyShoot[j].dy = 0;
                      break;
                    case 4:
                      manageBresenhamShoots (1, j);
                      break;
                    case 5:
                      manageBresenhamShoots (2, j);
                      break;
                    case 6:
                      manageBresenhamShoots (3, j);
                      break;
                    case 50:
                      manageBresenhamShoots (3, j);
                      break;
                    case 51:
                      manageBresenhamShoots (3, j);
                      break;
                    case 52:
                      manageBresenhamShoots (4, j);
                      break;
                    case 53:
                      manageBresenhamShoots (4, j);
                      break;
                    case 54:
                      manageBresenhamShoots (5, j);
                      break;
                    case 55:
                      manageBresenhamShoots (5, j);
                      break;
                    case 56:
                      manageBresenhamShoots (5, j);
                      break;
                    case 57:
                      manageBresenhamShoots (5, j);
                      break;
                    case 58:
                      manageBresenhamShoots (5, j);
                      break;
                    case 59:
                      manageBresenhamShoots (3, j);
                      break;
                    case 60:
                      manageBresenhamShoots (4, j);
                      break;
                    case 61:
                      manageBresenhamShoots (5, j);
                      break;
                    }
                }
            }
        }
    }

  /* shots's enemies displacement */
  for (n = 0; n < NBSHOOTSE; n++)
    {
      if (ennemyShoot[n].state)
        {
          int state =
            testShootCollisions (ennemyShoot[n].x + (int) ennemyShoot[n].dx,
                                 ennemyShoot[n].y + (int) ennemyShoot[n].dy);

          switch (state)
            {
            case 0:
              switch (ennemyShoot[n].type)
                {
                case 4:
                  manageBresenhamShoots (2, n);
                  break;
                case 5:
                  manageBresenhamShoots (3, n);
                  break;
                case 6:
                  manageBresenhamShoots (4, n);
                  break;
                }

              ennemyShoot[n].x += (int) ennemyShoot[n].dx;
              ennemyShoot[n].y += (int) ennemyShoot[n].dy;
              break;
            case 1:
              ennemyShoot[n].state = 0;
              addBurst (ennemyShoot[n].x, ennemyShoot[n].y);
              break;
            case 2:
              ennemyShoot[n].state = 0;
              addBurst (ennemyShoot[n].x, ennemyShoot[n].y);
              Die ();
              break;
            }
        }
    }
}

/**
 *
 */
int
manageGame (void)
{
  int done = 0;
  int first = 2;

  /* loadMusic(PAKNAME, REP_SND "tecend.mod"); */

  actu_score = actu_ammo = actu_life = actu_time = 2;
  NewTick = 0;
  while (!done)
    {

/* initialization parts */

      /* clear screen */
      clearBoard (screen);

/* PARTIE GESTION */

      if (apparition && first == 0 && player.life > 0
          && strcmp (cfg_animation, "on") == 0)
        {
          apparition = 0;
          manageApparition ();
        }

      delta_time = SDL_GetTicks () - NewTick;
      if (NewTick == 0)
        {
          NewTick += delta_time;
          delta_time = 15;
        }
      else
        {
          NewTick += delta_time;
          delta_time = 17;
        }
      /* Gere les appuis sur le clavier et la souris */
      manageKeys ();

      /* event handle */
      if (event[EVENT_QUIT])
       {
          done = 1;
       }
      if (player.life <= 0)
       {
          done = 2;
       }
      if (player.undying > 0)
       {
          player.undying--;
       }

      manageTransformation ();

      if (die == 0)
        manageEvents ();
      else
        {
          die--;

          if (die == 0)
            restart ();
        }

      manageShoots ();

      manageBonus ();
      manageBursts ();
      manageEnnemys ();
      manageStars ();
      manageCollapsingBlocks ();

      testMortal ();
      testDoors ();

      if (form == SPHEREFORM && CableLaunched)
        if (testRopeCollision ())
          CableLaunched = 0;

/* draw section */

      displayMap ();

      /* draw enemies */
      displayEnnemys ();

      displayShoots ();
      displayBursts ();
      displayStar ();
      displayCollapsingBlocks ();

      /* draw the Spoing */
      if (die == 0)
        {
          if (CableLaunched)
            {
              if (form == SPOINGFORM)
                {
                  displayCables ();
                }
              else
                {
                  displayRope ();
                }
            }
          displayPlayer ();
        }

      if (first > 0)
        {
          displayInfos2 ();
          first--;
        }
      /* display infos on the screen */
      displayInfos ();
/* end section */
      /* flip buffers */

      if (die == 1)
        {
          SDL_FreeSurface (oldscreen);
          oldscreen = SDL_DisplayFormat (screen);
        }

      SDL_Flip (screen);

    }

  stopSound ();

  return done;
}

void
manageBonus ()
{
  int i, j;

  for (i = (int) player.x / width * width;
       i <= ((int) player.x + player.w) / width * width; i += width)
    for (j = (int) player.y / height * height;
         j <= ((int) player.y + player.h) / height * height; j += height)
      if (getCollision (i, j) == COL_BONUS)
        {
          putCollision (i, j, COL_VIDE);
          NbBonus--;

          player.score += 150;
          actu_score = 2;
          player.ammo += 5;
          actu_ammo = 2;

          copyPicture (IMG_map, IMG_decor, width * column - width, 0, width,
                       height, i / width * width, j / height * height);

          playSound (4);

          displayShadow (IMG_decor, i / width, j / height);
        }

  if (NbBonus == 0)
    {
      openDoors ();
      Bonus[player.level] = 1;
    }

}

void
manageEvents ()
{
  int i, j, k;

  if (event[EVENT_FULLSCREEN])
    {
      fullscreen = !fullscreen;
      SDL_FreeSurface (screen);
      if ((screen =
           SDL_SetVideoMode (X, Y, COLOR_DEPTH,
                             SDL_SWSURFACE | SDL_DOUBLEBUF | (fullscreen ?
                                                              SDL_FULLSCREEN :
                                                              0))) == NULL)
        {
          fprintf (stderr, "Error while trying to toggle in %s mode",
                   (fullscreen ? "fullscreen" : "window"));
        }
    }

/* cheat section */
#ifdef CHEAT
  if (event[EVENT_NEXTMAP] && player.level < 99)
    {
      player.level++;
      initMap ();
    }

  if (event[EVENT_PRECMAP] && player.level > 0)
    {
      player.level--;
      initMap ();
    }
#endif

  /* Si on demande à lancer le cable, que ce dernier n'est pas encore lancé et que l'on est sur un sol quelconque */
  if (event[EVENT_PRESSEDCABLE] && !CableLaunched
      && (form == SPOINGFORM && testGroundCollisions ()))
    /* /manageCollisions(1, EVENT_DOWN, (int)player.x, (int)player.y+player.h, player.w, (int)player.dy+1+agrandissement))) */
    {
      j = (int) player.y + CANNONH - 4;

      /* On cherche le point d'accroche gauche, en parcourant la ligne gauche jusqu'à rencontrer une collision */
      i = (int) player.x / width * width;
      while (getCollision (i, j) == COL_VIDE
             && getCollision (i, j + 4) == COL_VIDE && i > 0)
        i -= width;
      /* On cherche le point d'accroche droit, en parcourant la ligne droite jusqu'à rencontrer une collision */
      k = ((int) player.x + player.w) / width * width;
      while (getCollision (k, j) == COL_VIDE
             && getCollision (k, j + 4) == COL_VIDE && k < X)
        k += width;

      /* Si on a bien les 2 points d'accroche (possibilité d'accrocher le cable), on accroche le cable */
      if (getCollision (i, j) == COL_NORMAL
          && getCollision (k, j) == COL_NORMAL)
        {
          XLeftCable = i + width;
          XRightCable = k;
          YLeftCable = YRightCable = player.y + CANNONH - 4;
          CableLaunched = 1;
          playSound (1);
        }
      else
        {
          playSound (5);
        }

    }

  /* Si on demande à lancer le cable, que ce dernier n'est pas encore lancé et que l'on est sur un sol quelconque */
  if (event[EVENT_PRESSEDCABLE] && !CableLaunched && (form == SPHEREFORM))
    {
      j = (int) (player.y) / height * height;

      /* On cherche le point d'accroche gauche, en parcourant la ligne gauche jusqu'à rencontrer une collision */
      i = ((int) player.x + player.w / 2) / width * width;
      while (getCollision (i, j) == COL_VIDE && j > 0)
        j -= height;

      /* Si on a bien les 2 points d'accroche (possibilité d'accrocher le cable), on accroche le cable */
      if (getCollision (i, j) == COL_NORMAL)
        {
          XCable = player.x + player.w / 2;
          YCable = j + height;
          CableLaunched = 1;
          ropew = fabs (player.y + player.h / 2 - YCable);
          playSound (1);
        }
      else
        playSound (5);
    }

  /* Si on lache la touche cable, on lache le cable */
  if (!event[EVENT_CABLE])
    CableLaunched = 0;

  /* On change la vitesse selon le deplacement voulu */
  if (event[EVENT_RIGHT])
    player.dx += (player.ax * (float) delta_time) / (float) 1024;

  if (event[EVENT_LEFT])
    player.dx -= (player.ax * (float) delta_time) / (float) 1024;

  /* iminue la taille du spoing si demandé et si possible (si on peut encore la diminuer) */
  if (event[EVENT_DOWN] && player.h > SPOINGHMIN && form == SPOINGFORM)
    {
      /* Si le cable n'est pas lancé, on change la taille et l'ordonnée du spoing */
      if (!CableLaunched && !testDownCollisions ())
        {
          agrandissement = 0;
          while (agrandissement < agrandissementMax
                 && player.h - agrandissement > SPOINGHMIN
                 && !testDownCollisions ())
            {
              agrandissement++;
              player.h--;
              player.y++;
            }
        }
      /* Si le cable est lancé, on change juste la taille */
      else
        {
          agrandissement = 0;
          while (agrandissement < agrandissementMax
                 && player.h - agrandissement > SPOINGHMIN
                 && !testUpCollisions2 ())
            {
              agrandissement++;
              player.h--;
            }
        }
    }

  /* Agrandit la taille du spoing si demandé et si possible (si il n'y a pas de collision) */
  if (event[EVENT_UP] && form == SPOINGFORM)
    {
      /* Si le cable n'est pas lancé et qu'il n'y a pas de collisions, 
         on change la taille et l'ordonnée du spoing */
      if (!CableLaunched && testGroundCollisions ())    /* manageCollisions(1, EVENT_DOWN, (int)player.x, (int)player.y+player.h, player.w, (int)player.dy+1+agrandissement)==1) */
        {
          agrandissement = 0;
          while (agrandissement < agrandissementMax && !testUpCollisions ())    /* manageCollisions(1, EVENT_UP, (int)player.x, (int)player.y, player.w, -1+(int)player.dy-1-agrandissement)==0) */
            {
              agrandissement++;
              player.h++;
              player.y--;
            }
        }
      /* Si le cable est lancé et qu'il n'y a pas de collisions,  */
      /* on change juste l'ordonnée du spoing */
      else
        {
          agrandissement = 0;
          while (agrandissement < agrandissementMax && !testGroundCollisions ())        /* manageCollisions(1, EVENT_DOWN, (int)player.x, (int)player.y+player.h, player.w, (int)player.dy+1+agrandissement)==0) */
            {
              agrandissement++;
              player.h++;
            }
        }
    }

  agrandissement = 0;

  /* Charge le saut : augmente ce dernier tant que la touche bas est appuyée
   * */
  if (event[EVENT_DOWN] && player.h < SPOINGH && !CableLaunched
      && form == SPOINGFORM && testGroundCollisions ())
    if (saut < AY)
      saut += AY / 50;

  if (form == SPHEREFORM && event[EVENT_DOWN] && !testGroundCollisions ())      /* !manageCollisions(1, EVENT_DOWN, (int)player.x, (int)player.y+player.h, player.w, (int)player.dy+2)) */
    {
      ropew++;
      player.y++;
    }

  if (event[EVENT_UP] && !testUpCollisions ())  /* !manageCollisions(1, EVENT_UP, (int)player.x, (int)player.y, player.w, -1+(int)player.dy-2)) */
    {
      if (ropew > 10)
        {
          ropew -= 2;
          player.y -= 2;
        }
    }

  /* Si le changement de taille du spoing n'est pas sollicité,
     On le fait revenir à la normale */
  if (!event[EVENT_UP] && !event[EVENT_DOWN] && form == SPOINGFORM)
    {
      /* Si la taille est trop grande, on la réduit */
      if (player.h > SPOINGH)
        {
          if (CableLaunched)
            {
              if (!testUpCollisions2 ())
                player.h--;
            }
          else
            /* Si le cable n'est pas lancé et que le spoing n'est pas bloqué, on change l'ordonnee */
            {
              if (!testDownCollisions ())
                {
                  player.y++;
                  player.h--;
                }
              else if (!testUpCollisions ())
                player.h--;

            }
        }

      /* Si la taille est trop petite, on l'agrandit */
      if (player.h < SPOINGH)
        {
          /* Si le cable est lancé, et qu'il n'y a pas de collision, on agrandit la taille */
          if (CableLaunched)    /* !manageCollisions(1, EVENT_DOWN, (int)player.x,(int)player.y+player.h, player.w, (int)player.dy+1+agrandissement)) */
            {
              if (!testDownCollisions ())
                player.h++;
            }
          else
            /* Si le cable n'est pas lancé, et qu'il n'y a pas de collision, on agrandit la taille et l'ordonnée */
            {
              if (!testUpCollisions ())
                {
                  player.y--;
                  player.h++;
                }
            }
        }
    }

  /* Si la touche bas est lachée et que le spoing est sur le sol, on lance la force de saut */
  if (!event[EVENT_DOWN] && testGroundCollisions () && player.h < SPOINGH && form == SPOINGFORM)        /* manageCollisions(1, EVENT_DOWN, (int)player.x, (int)player.y+player.h, player.w, (int)player.dy+1+agrandissement) && player.h<SPOINGH && form==SPOINGFORM) */
    {
      player.dy -= ((float) saut * (float) delta_time) / (float) 1024;
      if (saut != 0)
        playSound (6);
      saut = 0;
    }

  if (form == SPHEREFORM && event[EVENT_UP] && testGroundCollisions ()) /* manageCollisions(1, EVENT_DOWN, (int)player.x, (int)player.y+player.h, player.w, (int)player.dy+1)) */
    player.dy -= (float) (AY / 2) * (float) delta_time / (float) 1024;

  /* Si il y a collision alors que la vitesse est non nulle, on inverse cette dernière et on multiplie par l'amortissement */
  if (player.dx < 0 && testLeftCollisions ())   /* manageCollisions(1, EVENT_LEFT, (int)player.x, (int)player.y, (int)player.dx-1, player.h)!=0) */
    {
      player.dx =
        -player.dx * ((float) SHOCK * (float) delta_time / (float) 1024);
      if (player.dx > 0.5)
        playSound (3);
    }
  if (player.dx > 0 && testRightCollisions ())  /* manageCollisions(1, EVENT_RIGHT, (int)player.x+player.w, (int)player.y, (int)player.dx+1, player.h)!=0 ) */
    {
      player.dx =
        -player.dx * ((float) SHOCK * (float) delta_time / (float) 1024);
      if (player.dx < -0.5)
        playSound (3);
    }
  if (player.dy < 0 && testUpCollisions ())     /* manageCollisions(1, EVENT_UP, (int)player.x, (int)player.y, player.w, -1+(int)player.dy-1-agrandissement)!=0) */
    {
      player.dy =
        -player.dy * ((float) SHOCK * (float) delta_time / (float) 1024);
      if (player.dy > 0.5)
        playSound (3);
    }

  if (player.dy > 0 && testDownCollisions ())   /* manageCollisions(1, EVENT_DOWN, (int)player.x, (int)player.y+player.h, player.w, (int)player.dy+1+agrandissement)!=0) */
    {
      player.dy =
        -player.dy * ((float) SHOCK * (float) delta_time / (float) 1024);
      if (player.dy < -0.5)
        playSound (3);
    }

  /* On ajoute la gravité... */
  if (!testGroundCollisions ()) /* manageCollisions(1, EVENT_DOWN, (int)player.x, (int)player.y+player.h, player.w, (int)player.dy+1+agrandissement)==0) */
    player.dy += ((float) GRAVITY * (float) delta_time / (float) 1024);

  /* On ajouteFX; les frottements de l'air */
  player.dx *= (float) FX *(float) delta_time / (float) 1024;
  player.dy *= (float) FY *(float) delta_time / (float) 1024;
  /*
     if (fabs(player.dx)<player.ax*5/10) player.dx=0;
     if (fabs(player.dy)<GRAVITY*5/10) player.dy=0;
   */

  /* Si le cable est lancé, il n'y pas de vitesse verticale... */
  if (CableLaunched && form == SPOINGFORM)
    player.dy = 0;

  /* On change les coordonnées du spoing en fonction des vitesses */
  player.x += (int) player.dx;
  player.y += (int) player.dy;

  if (CableLaunched && form == SPHEREFORM)
    moveRope ();

  /* Gestion du playerShoot */
  int flag_playerShoot = 1;
  static int waitplayerShoot = 0;
  if (event[EVENT_TIR]
      && ((form == SPOINGFORM && !CableLaunched) || form == SPHEREFORM))
    {
      if (player.ammo > 0)
        {
          /* Permet de ralenplayerShoot un peu la cadence de playerShoot par rapport aux fps... */
          if (waitplayerShoot >= 5)
            {
              playSound (0);

              actu_ammo = 2;

              for (i = 0; i < NBSHOOTS && flag_playerShoot; i += 2)
                {
                  if (playerShoot[i].state == 0
                      && playerShoot[i + 1].state == 0)
                    {
                      player.ammo--;
                      playerShoot[i].state = playerShoot[i + 1].state = 1;
                      playerShoot[i].power = 1;
                      playerShoot[i + 1].power = 1;
                      playerShoot[i].x = (int) player.x - FireW;
                      playerShoot[i + 1].x = (int) player.x + player.w;
                      playerShoot[i].y = playerShoot[i + 1].y =
                        (int) player.y + CANNONH - 6;

                      flag_playerShoot = 0;
                    }
                }
              waitplayerShoot = 0;
            }
          else
            waitplayerShoot++;
        }
    }

  int FireSpeed =
    (int) ((float) FIRESPEED * (float) delta_time / (float) 1024);

  /* Gestion des collisions des playerShoots du spoing sur un ennemi */
  for (i = 0; i < NBSHOOTS; i++)
    if (playerShoot[i].state)
      for (j = 0; j < NBENNEMYS; j++)
        if (ennemy[j].x != 0)
          if (testCollisions
              (playerShoot[i].x + (i % 2 == 0 ? -FireSpeed : 0),
               playerShoot[i].y, FireW + FireSpeed, FireH, ennemy[j].x,
               ennemy[j].y, ennemy[j].w, ennemy[j].h))
            {
              ennemy[j].resistance -= playerShoot[i].power;
              ennemy[j].mask = 10;

              playerShoot[i].state = 0;
              addBurst (playerShoot[i].x, playerShoot[i].y);
              if (ennemy[j].resistance <= 0)
                {
                  addBigBurst (ennemy[j].x, ennemy[j].y);
                  ennemy[j].x = 0;
                  player.score += ennemy[j].bonus * 100;
                  actu_score = 2;
                }

            }

  if (ennemyCollisions (EVENT_LEFT) && (int) player.dx == 0)
    {
      if (manageCollisions
          (0, EVENT_RIGHT, (int) player.x + player.w, (int) player.y, 1,
           player.h))
        {
          Die ();
        }
      else
        {
          player.x++;
        }
    }
  if (ennemyCollisions (EVENT_RIGHT) && (int) player.dx == 0)
    {
      if (manageCollisions
          (0, EVENT_LEFT, (int) player.x, (int) player.y, -1, player.h))
        {
          Die ();
        }
      else
        {
          player.x--;
        }
    }
  if (ennemyCollisions (EVENT_DOWN) && (int) player.dy == 0)
    {
      if (testUpCollisions () || CableLaunched)
        {
          if (!CableLaunched && player.h > SPOINGHMIN)
            {
              player.h--;
            }
          else
            {
              Die ();
            }
        }
      else
        {
          player.y--;
        }
    }
  if (ennemyCollisions (EVENT_UP) && (int) player.dy == 0)
    {
      if (testDownCollisions () || CableLaunched)
        {
          if (!CableLaunched && player.h > SPOINGHMIN)
            {
              player.h--;
            }
          else
            {
              Die ();
            }
        }
      else
        {
          player.y++;
        }
    }

  /* clear empty list */
  for (i = 0; i < 100; i++)
    event[i] = 0;
}

void
manageTransformation ()
{
  int i, j;

  for (i = (int) (player.x) / width * width;
       i <= ((int) player.x + player.w) / width * width; i += width)
    for (j = (int) (player.y) / height * height;
         j <= ((int) player.y + player.h) / height * height; j += height)
      {
        if (getCollision (i, j) == COL_TRANS_SB && !CableLaunched
            && form == SPOINGFORM)
          {
            /* Permits not to be out of the board */
            player.y += player.h;

            form = SPHEREFORM;
            player.w = SPHEREW;
            player.h = SPHEREH;

            /* Permits not to be out of the board */
            player.y -= player.h + 2;
            addStars ();
          }
        if (getCollision (i, j) == COL_TRANS_BS && !CableLaunched
            && form == SPHEREFORM)
          {
            /* Permits not to be out of the board */
            player.y += player.h;

            form = SPOINGFORM;
            player.w = SPOINGW;
            player.h = SPOINGH;

            /* Permits not to be out of the board */
            player.y -= player.h + 2;

            initStar ();
          }
      }
}

void
manageApparition ()
{
  int i, j, k;
  int wou;
  int flag;
  SDL_Surface *tmp_player;

  int Xarray[SPOINGW][SPOINGH], Yarray[SPOINGW][SPOINGH];

  tmp_player =
    SDL_CreateRGBSurface (SDL_SWSURFACE, player.w, player.h, COLOR_DEPTH, 0,
                          0, 0, 0);
  SDL_SetColorKey (tmp_player, SDL_SRCCOLORKEY,
                   SDL_MapRGB (IMG_spoing->format, 4, 2, 4));

  clearSurface (tmp_player);

  /*! Copy the player in a surface */
  if (form == SPOINGFORM)
    {
      copyPicture (IMG_spoing, tmp_player, 0, 0, player.w, HEADH, 0, 0);
      copyPicture (IMG_body, tmp_player, 0, 0, player.w, player.h - HEADH - 6,
                   4, HEADH);
      copyPicture (IMG_spoing, tmp_player, 0, FOOTH, player.w, 6, 0,
                   player.h - 6);
    }
  else
    {
      copyPicture (IMG_spoing, tmp_player, 255, 0, player.w, player.h, 0, 0);
    }

  wou = rand () % 5;
  for (j = 0; j < player.w; j++)
    for (k = 0; k < player.h; k++)
      {
        switch (wou)
          {
          case 0:
            Xarray[j][k] = (int) player.x + j;  /* player.w/2-(1000/player.w)*(player.w/2-j); */
            Yarray[j][k] =
              (int) player.y + player.h / 2 -
              (1000 / player.h) * (player.h / 2 - k);
            break;
          case 1:
            Xarray[j][k] =
              (int) player.x + player.w / 2 -
              (1000 / player.w) * (player.w / 2 - j);
            Yarray[j][k] = (int) player.y + k;
            break;
          case 2:
            Xarray[j][k] =
              (int) player.x + player.w / 2 -
              (1000 / player.w) * (player.w / 2 - j);
            Yarray[j][k] =
              (int) player.y + player.h / 2 -
              (1000 / player.h) * (player.h / 2 - k);
            break;
          case 3:
          case 4:
            Xarray[j][k] = rand () % X;
            Yarray[j][k] = rand () % (line * height);
            break;
          }
      }

  int dx, dy;

  do
    {
      flag = 0;

      displayMap ();
      /* displayInfos2(); */

      for (i = 0; i < player.w; i++)
        for (j = 0; j < player.h; j++)
          {
            dx = (int) player.x + i > Xarray[i][j] ? 1 : -1;

            dy = (int) player.y + j > Yarray[i][j] ? 1 : -1;

            if (Xarray[i][j] != (int) player.x + i)
              Xarray[i][j] += dx;
            if (Yarray[i][j] != (int) player.y + j)
              Yarray[i][j] += dy;

            if (Xarray[i][j] != (int) player.x + i
                || Yarray[i][j] != (int) player.y + j)
              flag = 1;

            if (Xarray[i][j] > 0 && Xarray[i][j] < X && Yarray[i][j] > 0
                && Yarray[i][j] < line * height)
              copyPicture (tmp_player, screen, i, j, 1, 1, Xarray[i][j],
                           Yarray[i][j]);
          }

      SDL_Flip (screen);

      /* SDL_Delay(10); */

    }
  while (flag);

  SDL_FreeSurface (tmp_player);

}
