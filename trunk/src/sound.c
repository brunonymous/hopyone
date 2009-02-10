/**************************************************
                    INCLUDES
**************************************************/

#include "global.h"

/**************************************************
                    VARIABLES
**************************************************/

Sint32 soundvol, musicvol;

Mix_Music *mod = NULL;

Mix_Chunk *samples[20];

/**************************************************
                    FUNCTIONS
**************************************************/

void initSound()
{
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512*4*2)<0)
        fprintf(stderr, "sound init failed (SDL_mixer): %s", Mix_GetError());
}

void stopSound()
{
    if(mod)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(mod);
        mod = NULL;
    }
}

void cleanSound()
{
    Sint32 i;
    
    stopSound();
    
    for (i=0;i<20;i++)
       if (samples[i])
          Mix_FreeChunk(samples[i]);
    
    Mix_CloseAudio();
}

void loadMusic(char *pak, char *name)
{
    if(soundvol && musicvol && strcmp(cfg_music, "on")==0)
    {
        stopSound();
        
        mod = loadMusicFromZip(pak, name); /* Mix_LoadMUS(name); / */
        if(mod)
        {
            Mix_PlayMusic(mod, -1);
            Mix_VolumeMusic(musicvol);
        }
    }
}

void openSound(char *pak, char *name, Sint32 n)
{
     samples[n] = loadSoundFromZip(pak, name); /* Mix_LoadWAV(name); */
     
     if(!samples[n]) 
     { 
        fprintf(stderr, "failed to load sample: %s", name); 
        exit(0);
     }
}

void playSound(Sint32 n)
{
    Sint32 chan;
    if(!soundvol || strcmp(cfg_sound, "on")!=0) 
       return;
    
    chan = Mix_PlayChannel(-1, samples[n], 0);
    
    if(chan>=0) 
       Mix_Volume(chan, soundvol);
}

