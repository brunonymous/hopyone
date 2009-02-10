/**
 * @file ini.c 
 * @brief Main function is where the program starts execution 
 * @created 2007-11-12 
 * @date 2009-02-10 
 */
/*
 * copyright (c) 1998-2009 TLK Games all rights reserved
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

char cfg_lang[64], cfg_text[64], cfg_animation[64];
char cfg_music[64], cfg_sound[64], buffer[64];

static const char SetupFile[] = "hopyone.ini";
static const char SetupFile2[] = "config.ini";
static const char OldSetupFile[] = "old_hopyone.ini";

/**
 * Remove spaces and line end of a string, and non-displayable chars
 * @param src
 * @par dst
 */
static void
CleanString (const char *src, char *dst)
{
  const char* psrc;
  char *pdst;
  Uint32 n;
  psrc = src;
  pdst = dst;
  for (n = 0; n < strlen (src); n++)
    {
      if ((*psrc > ' '))
        {
          *pdst = *psrc;
          pdst++;
        }
      psrc++;
    }
  /* end of string */
  *pdst = 0;
}

// découpe une chaine '%s = %s'
int
CutConfigLine (char *str, char *name, char *value)
{
  char *pstr;
  int p;
  if ((str == NULL) || (strlen (str) == 0))
    {
      return 0;
    }

  pstr = strchr (str, '=');
  if (pstr == NULL)
    return 0;
  else
    {
      p = (pstr - str);
      memcpy (name, str, p);
      *(name + p) = 0;
      memcpy (value, pstr + 1, strlen (str) - p - 1);
      *(value + strlen (str) - p - 1) = 0;
      return 1;
    }
}

/**
 * Retrieve a value in a *.ini file
 */ 
static int
GetConfigString (const char *file, const char *name, char *value)
{
  FILE *f;
  int eof;
  int fnd;
  char str[256];
  char tmp[256];
  char cfgname[256], cfgvalue[256];

  f = fopen (file, "rt");
  if (f == NULL)
   {
    return 0;
   }

  eof = 0;
  fnd = 0;
  while ((!eof) && (!fnd))
    {
      /* reading a line */
      fgets (str, 256, f);

      if (!feof (f))
        {                       // test de fin de fichier

          if (strlen (str) > 2)
            {                   // enleve les lignes vides

              if ((str[0] != '/') && (str[1] != '/'))
                {               // enleve les commentaires

                  // enleve les caractères spéciaux
                  CleanString (str, tmp);

                  // découpe en nom et valeur
                  if (CutConfigLine (tmp, cfgname, cfgvalue))
                    {

                      // comparaison avec le nom recherché
                      if (strcmp (name, cfgname) == 0)
                        {
                          memcpy (value, cfgvalue, strlen (cfgvalue) + 1);
                          fnd = 1;
                        }
                    }
                }
            }
        }
      else
        eof = 1;
    }

  fclose (f);


  return fnd;
}

// lecture du fichier de configuration du jeu
int
ReadConfigFile ()
{
  //char tmp[256],tmpf[256],real[2048];

  // lang
  if (!GetConfigString (SetupFile, "Lang", cfg_lang))
    {
      sprintf (cfg_lang, "english");
    }

  // Text
  if (!GetConfigString (SetupFile, "Text", cfg_text))
    {
      sprintf (cfg_text, "normal");
    }

  // Animation
  if (!GetConfigString (SetupFile, "Animation", cfg_animation))
    {
      sprintf (cfg_animation, "on");
    }

  // Music
  if (!GetConfigString (SetupFile, "Music", cfg_music))
    {
      sprintf (cfg_music, "on");
    }

  // Sound
  if (!GetConfigString (SetupFile, "Sound", cfg_sound))
    {
      sprintf (cfg_sound, "on");
    }

  // Sound volume
  if (!GetConfigString (SetupFile, "MusicVol", buffer))
    {
      musicvol = 100;
    }
  else
    musicvol = atoi (buffer);

  // Sound volume
  if (!GetConfigString (SetupFile, "SoundVol", buffer))
    {
      soundvol = 100;
    }
  else
    {
      soundvol = atoi (buffer);
    }

  return 1;
}

// lecture du fichier de configuration du jeu
int
ReadConfigFile2 ()
{
  // FX
  if (GetConfigString (SetupFile2, "FX", buffer))
    {
      FX = atoi (buffer);
    }

  // FY
  if (GetConfigString (SetupFile2, "FY", buffer))
    {
      FY = atoi (buffer);
    }

  // GRAVITY
  if (GetConfigString (SetupFile2, "GRAVITY", buffer))
    {
      GRAVITY = atoi (buffer);
    }

  // SHOCK
  if (GetConfigString (SetupFile2, "SHOCK", buffer))
    {
      SHOCK = atoi (buffer);
    }

  // AX
  if (GetConfigString (SetupFile2, "AX", buffer))
    {
      AX = atoi (buffer);
    }

  // AY
  if (GetConfigString (SetupFile2, "AY", buffer))
    {
      AY = atoi (buffer);
    }
  // FIRESPEED
  if (GetConfigString (SetupFile2, "FIRESPEED", buffer))
    {
      FIRESPEED = atoi (buffer);
    }

  return 1;
}

/*! Loads the texts of the game */
void
loadText ()
{
  char tab[10000];
  char buffer[100];

  sprintf (buffer, "%s%s.lng", REP_LANG, cfg_lang);

  SDL_RWops *op;
  if ((op = TLK_RWFromZip (PAKNAME, buffer)) != NULL)
    {
      SDL_RWread (op, &tab, sizeof (char), 10000);
    }

  char car;
  int i = 0, j = 0;
  int k = 0;
  do
    {
      car = tab[k++];           //fgetc(file);

      if (car != '\0' && car != '\n')
        {
          text[i][j] = car;
          j++;
        }
      if (car == '\n')
        {
          text[i][j] = '\0';
          i++;
          j = 0;
        }
    }
  while (tab[k] != '\0');       //!feof(file));
}

void
saveINI ()
{
  FILE *file, *newfile;
  int i;
  char str[256];

  file = fopen (SetupFile, "rt");
  newfile = fopen (OldSetupFile, "wt");

  while (!feof (file))
    {
      fgets (str, 256, file);
      if (!feof (file))
        fputs (str, newfile);
    }

  fclose (file);
  fclose (newfile);

  file = fopen (OldSetupFile, "rt");
  newfile = fopen (SetupFile, "wt");

  while (!feof (file))
    {
      fgets (str, 256, file);

      if (!feof (file))
        {
          if (strlen (str) <= 2 || (str[0] == '/' && str[1] == '/'))
            fputs (str, newfile);
          else
            {
              char buffer[100];

              i = 0;
              while (str[i] != '=')
                {
                  buffer[i] = str[i];
                  i++;
                }
              buffer[i] = '\0';

              if (strcmp (buffer, "Lang") == 0)
                sprintf (str, "%s=%s\n", buffer, cfg_lang);
              if (strcmp (buffer, "Text") == 0)
                sprintf (str, "%s=%s\n", buffer, cfg_text);
              if (strcmp (buffer, "Animation") == 0)
                sprintf (str, "%s=%s\n", buffer, cfg_animation);
              if (strcmp (buffer, "Music") == 0)
                sprintf (str, "%s=%s\n", buffer, cfg_music);
              if (strcmp (buffer, "Sound") == 0)
                sprintf (str, "%s=%s\n", buffer, cfg_sound);
              if (strcmp (buffer, "MusicVol") == 0)
                sprintf (str, "%s=%d\n", buffer, musicvol);
              if (strcmp (buffer, "SoundVol") == 0)
                sprintf (str, "%s=%d\n", buffer, soundvol);

              fputs (str, newfile);
            }
        }
    }

}
