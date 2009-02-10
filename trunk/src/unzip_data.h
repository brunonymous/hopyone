#ifndef __UNZIP_DATA__
#define __UNZIP_DATA__

#include <SDL.h>
#include <SDL_rwops.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "unzip.h"

extern  int zip_close(SDL_RWops *context);

extern SDL_RWops * TLK_RWFromZip(char * pak, char * filename);

extern SDL_Surface *loadPictureFromZip(char * pak, char * filename);

extern FILE *loadFileFromZip(char * pak, char * filename);

extern TTF_Font *loadFontFromZip(char *pak, char *filename, int size);

extern Mix_Chunk * loadSoundFromZip(char *pak, char *name);
extern Mix_Music * loadMusicFromZip(char *pak, char *filename);

#endif
