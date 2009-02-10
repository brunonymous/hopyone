/**
 * @file ini.h 
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
#ifndef _ini_h
#define _ini_h
#define TEXT_SCORE 0
#define TEXT_TIME 1
#define TEXT_LIFE 2
#define TEXT_AMMO 3
#define TEXT_START 4
#define TEXT_INSTRUCTIONS 5
#define TEXT_LOADSAVE 6
#define TEXT_CONFIGURATION 7
#define TEXT_CREDITS 8
#define TEXT_CATALOG 9
#define TEXT_ORDERING 10
#define TEXT_SCORES 11
#define TEXT_EXIT 12
#define TEXT_FILENAME 13
#define TEXT_SAVE 14
#define TEXT_LOAD 15
#define TEXT_SELECTFILE 16
#define TEXT_LEVEL 17
#define TEXT_DIE 18
#define TEXT_GAMEOVER 19
#define TEXT_HISCORE 20
#define TEXT_ENTERNAME 21
#define TEXT_LANG 22
#define TEXT_TEXT 23
#define TEXT_ANIMATION 24
#define TEXT_MUSIC 25
#define TEXT_SOUND 26
#define TEXT_MUSICVOLUME 27
#define TEXT_SOUNDVOLUME 28

extern char cfg_lang[64], cfg_text[64], cfg_animation[64];
extern char cfg_music[64], cfg_sound[64];


/* lecture du fichier de configuration du jeu */
Sint32 ReadConfigFile (void);

Sint32 ReadConfigFile2 (void);

/*! Loads the texts of the game */
void loadText (void);

void saveINI (void);

#endif

