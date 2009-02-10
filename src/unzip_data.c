/**
 * @file unzip_data.c 
 * @brief 
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


#include "unzip.h"
#include "unzip_data.h"

/**
  Create a RWop from a file in a zip
  pak is the name of the .zip
  filename is the name of the file in the .zip
  ex. TLK_RWFromZip("data.zip","test.bmp");

  @param pak name of the .zip to read from
  @param filename name of the file to read in the .zip
  @remarks if a file with the same filename is present <b>outside</b> the 
.zip, it will be opened instead of any file in the .zip
*/
SDL_RWops *
TLK_RWFromZip (char *pak, char *filename)
{
  void *zip;
  int size;
  SDL_RWops *rw;
  // // FILE *f;

  //fprintf(stdout, "unzip_data.c/TLK_RWFromZip(%s)\n", filename);

  rw = SDL_RWFromFile (filename, "rb");
  /* if a regular file is found, use it instead of reading from zip */
  if (rw != NULL)
    {
      return rw;
    }

  zip = unzOpen (pak);
  /* zip found ? */
  if (zip == NULL)
    {
      fprintf (stderr, "%s zip file not found!\n", filename);
      exit (1);
    }

  /* filename is in zip ?
     search is case-insensitive */
  if (unzLocateFile (zip, filename, 2) != UNZ_OK)
    {
      fprintf (stderr, "file \"%s\" not found in zip!\n", filename);
      exit (1);
    }
  /* return NULL; */

  unz_file_info info;
  unzGetCurrentFileInfo (zip, &info, NULL, 0, NULL, 0, NULL, 0);
  size = info.uncompressed_size;

  if (unzOpenCurrentFile (zip) != UNZ_OK)
    {
      fprintf (stderr, "error while opening file !\n");
      exit (1);
    }
  /* return NULL; */

  Uint8 *buffer;
  buffer = (Uint8 *) malloc (size);

  int result;
  result = unzReadCurrentFile (zip, buffer, size);

  /* fprintf(stdout, "%s : size = %d, result = %d\n", filename, size, result); */

  rw = SDL_RWFromMem (buffer, size);

  /* destructor */
  rw->close = zip_close;

  if (size != result)
    {
      free (buffer);
      /* return NULL; */
      fprintf (stderr, "error while extracting file %s from the zip !\n",
               filename);
      exit (1);
    }

  unzCloseCurrentFile (zip);
  unzClose (zip);
  return rw;
}

/**
	internal use: free a RWop created from a zip
*/
int
zip_close (SDL_RWops * context)
{
  if (context != NULL)
    {
      free (context->hidden.mem.base);
    }
  return 0;
}

SDL_Surface *
loadPictureFromZip (char *pak, char *filename)
{
  SDL_Surface *bitmap = NULL;
  SDL_RWops *op;

  if ((op = TLK_RWFromZip (pak, filename)) != NULL)
    {
      bitmap = IMG_Load_RW (op, SDL_FALSE);
      if (bitmap == NULL)
        {
            fprintf(stderr, "unzip_data.c/loadPictureFromZip()"
                    " (!)IMG_Load_RW() return %s\n", IMG_GetError());
        }
      op->close (op);
    }
  else
    {
      fprintf (stderr, "Error while trying to load %s/%s\n", pak, filename);
      exit (1);
    }

  return bitmap;
}

FILE *
loadFileFromZip (char *pak, char *filename)
{
  SDL_RWops *op;
  // // FILE *fic;
  int poil;

  if ((op = TLK_RWFromZip (pak, filename)) != NULL)
    {
      SDL_RWread (op, &poil, sizeof (int), 1);
      return op->hidden.stdio.fp;
    }
  else
    {
      fprintf (stderr, "Error while trying to load %s/%s\n", pak, filename);
      exit (1);
    }
}

TTF_Font *
loadFontFromZip (char *pak, char *filename, int size)
{
  SDL_RWops *op;

  if ((op = TLK_RWFromZip (pak, filename)) != 0)
    {
      TTF_Font *font;

      font = TTF_OpenFontRW (op, SDL_FALSE, size);
      /* op->close(op); */

      return font;
    }
  else
    {
      fprintf (stderr, "Error while trying to load %s/%s\n", pak, filename);
      exit (1);
    }

}

Mix_Chunk *
loadSoundFromZip (char *pak, char *filename)
{
  SDL_RWops *op;

  char name[255];

  if ((op = TLK_RWFromZip (pak, filename)) != 0)
    {
      Mix_Chunk *chunk;

      chunk = Mix_LoadWAV_RW (op, 1);
      /* op->close(op); */

      if (!chunk)
        {
          fprintf (stderr, "failed to load sample: %s", filename);
          exit (0);
        }

      return chunk;
    }
  else
    {
      fprintf (stderr, "Error while trying to load %s/%s\n", name, filename);
      exit (1);
    }
}

Mix_Music *
loadMusicFromZip (char *pak, char *filename)
{
  SDL_RWops *op;

  char name[255];

  if ((op = TLK_RWFromZip (pak, filename)) != 0)
    {
      Mix_Music *music;

      /* FONCTION  NON ENCORE CREEEE !!! */
      /* music =  Mix_LoadMUS_RW(op); */

      /* op->close(op); */

      if (!music)
        {
          fprintf (stderr, "failed to load music: %s", filename);
          exit (0);
        }

      return music;
    }
  else
    {
      fprintf (stderr, "Error while trying to load %s/%s\n", name, filename);
      exit (1);
    }
}
