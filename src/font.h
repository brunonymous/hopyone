/**
 * @file font.h 
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

#ifndef _font_h
#define _font_h

extern TTF_Font *TTFfont, *TTFfontmenu, *TTFfontmenu2;

extern SDL_Surface *SDLfont;


int centerText (TTF_Font * font, char buffer[], Sint32 beg, Sint32 end);
void printText (SDL_Surface * surface, TTF_Font * font, char buffer[],
                Sint32 x, Sint32 y, Sint32 r, Sint32 g, Sint32 b);
void printTextShaded (SDL_Surface * surface, TTF_Font * font, char buffer[],
                      Sint32 x, Sint32 y, Sint32 r, Sint32 g, Sint32 b);
void printTextBlended (SDL_Surface * surface, TTF_Font * font, char buffer[],
                       Sint32 x, Sint32 y, Sint32 r, Sint32 g, Sint32 b);

#endif
