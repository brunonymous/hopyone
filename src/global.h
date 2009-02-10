/**
 * @file global.h 
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

#ifndef _global_h
#define _global_h


#ifndef __cplusplus
#if defined(HAVE_STDBOOL_H)
/*
 * The C language implementation does correctly provide the standard header
 * file "stdbool.h".
 */
#include <stdbool.h>
#else

/*
 * The C language implementation does not provide the standard header file
 * "stdbool.h" as required by ISO/IEC 9899:1999.  Try to compensate for this
 * braindamage below.
 */
#if !defined(bool)
#define bool    int
#endif
#if !defined(true)
#define true    1
#endif
#if !defined(false)
#define false   0
#endif
#endif
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <math.h>
#include <zlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>

#include "general.h"
#include "hopy.h"
#include "ini.h"
#include "scores.h"
#include "collisions.h"
#include "font.h"
#include "burst.h"
#include "manage.h"
#include "rope.h"
#include "sound.h"
#include "oldhopy.h"
#include "compteur.h"
/* #include "scriptpage.h" */
#include "unzip_data.h"

/* Resolution's definitions */
#define X 1000
#define Y 768
#define COLOR_DEPTH 16

/* Game board's definitions */
#define column 40
#define line 35
#define width 25
#define height 20

/* Directory's definitions */
#define REP_COL  "COLS/"
#define REP_MAP  "MAPS/"
#define REP_GFX  "GFX/"
#define REP_LANG "LANGS/"
#define REP_FONT "FONTS/"
#define REP_SAVE "SAVES/"
#define REP_SND "SOUNDS/"
#define PAKNAME "datas.zip"

/* Events' definitions */
#define EVENT_LEFT          1
#define EVENT_RIGHT         2
#define EVENT_UP            3
#define EVENT_DOWN          4
#define EVENT_CABLE         5
#define EVENT_TIR           6
#define EVENT_ENTER         7

#define EVENT_PRESSEDLEFT   11
#define EVENT_PRESSEDRIGHT  12
#define EVENT_PRESSEDUP     13
#define EVENT_PRESSEDDOWN   14
#define EVENT_PRESSEDCABLE  15
#define EVENT_PRESSEDTIR    16
#define EVENT_PRESSEDENTER  17

#define EVENT_PRECMAP       20
#define EVENT_NEXTMAP       21
#define EVENT_CONSOLE       22
#define EVENT_FULLSCREEN    30
#define EVENT_CLIC          40
#define EVENT_CLICDOWN      41
#define EVENT_MOTION        42
#define EVENT_QUIT          99

/* Others definitions... */
/* #define debug */

/* Surfaces */
extern SDL_Surface *IMG_map, *IMG_spoing, *IMG_cable, *IMG_fire, *IMG_body, *IMG_mouse;

/* Decor */
extern SDL_Surface *IMG_decor, *IMG_old_decor, *gameover, *fond_menu;

extern Sint32 actu_score, actu_ammo, actu_time, actu_life;


#endif
