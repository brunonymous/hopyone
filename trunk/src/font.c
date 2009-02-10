/**
 * @file font.c 
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

#include "global.h"
TTF_Font *TTFfont = NULL, *TTFfontmenu = NULL, *TTFfontmenu2 = NULL;
SDL_Surface *SDLfont = NULL;

/**
 * Function permitting to get the correct absciss for centering a text string
 * @param font the font you use
 * @param buffer the text you want to display
 * @param beg the beginning of you area
 * @param end the ending of your area
 * @return
 */
Sint32
centerText (TTF_Font * font, char buffer[], Sint32 beg, Sint32 end)
{
  Sint32 w, h;

  TTF_SizeText (font, buffer, &w, &h);

  return (beg + ((end - beg - w) / 2));
}

void
printText (SDL_Surface * surface, TTF_Font * font, char buffer[], Sint32 x,
           Sint32 y, Sint32 r, Sint32 g, Sint32 b)
{
  SDL_Surface *SDLfont;

  SDL_Color color;
  color.r = r;
  color.g = g;
  color.b = b;

  if (strcmp (cfg_text, "normal") == 0)
    {
      SDLfont = TTF_RenderText_Solid (font, buffer, color);
    }
  else
    {
      SDLfont = TTF_RenderText_Blended (font, buffer, color);
    }
  copyPicture (SDLfont, surface, 0, 0, X, Y, x, y);
  SDL_FreeSurface (SDLfont);
}

void
printTextShaded (SDL_Surface * surface, TTF_Font * font, char buffer[],
                 Sint32 x, Sint32 y, Sint32 r, Sint32 g, Sint32 b)
{
  printTextBlended (surface, font, buffer, x + 2, y + 2, r > 50 ? r - 50 : 0,
                    g > 50 ? g - 50 : 0, b > 50 ? b - 50 : 0);
  printTextBlended (surface, font, buffer, x, y, r, g, b);
}

void
printTextBlended (SDL_Surface * surface, TTF_Font * font, char buffer[],
                  Sint32 x, Sint32 y, Sint32 r, Sint32 g, Sint32 b)
{
  SDL_Surface *SDLfont;

  SDL_Color color;
  color.r = r;
  color.g = g;
  color.b = b;
  SDLfont = TTF_RenderText_Blended (font, buffer, color);
  copyPicture (SDLfont, surface, 0, 0, X, Y, x, y);
  SDL_FreeSurface (SDLfont);
}
