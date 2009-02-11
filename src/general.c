/**
 * @file general.c 
 * @brief Main function is where the program starts execution 
 * @created 2007-11-12 
 * @date 2009-02-11 
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

StructMouse mouse;

/** Permits you to switch fullscrenn on/oof */
bool fullscreen = false;


//Timer
int NewTick = 0, LastTick = 0;

//Number of frames per seconde
int fps;

//Screen buffer
SDL_Surface *screen = NULL;
SDL_Surface *oldscreen = NULL;

//Events' list
int event[100];

//Games' texts in the chosen language
char text[40][255];

/**************************************************
                    FUNCTIONS
**************************************************/

/** This function permits you to initialize SDL
* \param windowName : The name of the window
* \param iconame : The destination of the icon image
*/
SDL_Surface *
initSDL (char *windowName, char *iconame)
{
  //Surface containing the icon picture.
  SDL_Surface *ico;
  //Surface containing the screen buffer
  SDL_Surface *surface;

  //Initializes SDL modes : video, timer and audio
  if (SDL_Init
      (SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) <
      0)
    {
      fprintf (stderr, "Error while initializing SDL : %s\n",
               SDL_GetError ());
      exit (1);
    }

  //Links SDL_Quit with the exit call of the window
  atexit (SDL_Quit);

  //Sets the video mode and creates the screen buffer
  surface =
    SDL_SetVideoMode (X, Y, COLOR_DEPTH,
                      SDL_SWSURFACE | SDL_DOUBLEBUF | (fullscreen ?
                                                       SDL_FULLSCREEN : 0));
  if (surface == NULL)
    {
      fprintf (stderr, "Error while setting the video mode %dx%dx%d : %s\n",
               X, Y, COLOR_DEPTH, SDL_GetError ());
      SDL_Quit ();
      exit (2);
    }

  //Loads the icon picture
  if ((ico = loadPictureFromZip (PAKNAME, iconame)) == NULL)
    {
      fprintf (stderr, "Error while loading the file %s : %s\n", iconame,
               SDL_GetError ());
      SDL_Quit ();
      exit (2);
    }

  //Pastes the icon in the window
  SDL_WM_SetIcon (ico, NULL);

  //Writes the window name in the window
  SDL_WM_SetCaption (windowName, NULL);

  //Frees the surface created for the icon
  SDL_FreeSurface (ico);

  //Hides the cursor
  SDL_ShowCursor (0);

  //Enables the UNICODE
  SDL_EnableUNICODE (1);

  return surface;
}

/** This function permits you to load a picture in "any" format thanks to SDL_image
* The by default SDL_image dll with the game supports only bmp and png.
* But you can use another SDL_image dll, with more formats, and consequently more disk space used...
* \param filename : The location of the picture
*/
SDL_Surface *
loadPicture (char *filename)
{
  SDL_Surface *tmp;

  if ((tmp = loadPictureFromZip (PAKNAME, filename)) == NULL)
    {
      fprintf (stderr, "Error while loading file %s !\n", filename);
      exit (1);
    }
  return tmp;
}

/** This function permits you to display a full square in a surface
* \param surface : The surface you want to draw
* \param squarex : The abscissa of the square
* \param squarey : The ordonnee of the square
* \param squarew : The width of the square
* \param squareh : The height of the square
* \param r : The red variant
* \param g : The green variant
* \param b : The blue variant
*/
void
displayFullSquare (SDL_Surface * surface, int squarex, int squarey,
                   int squarew, int squareh, int r, int g, int b)
{
  Uint32 color;
  SDL_Rect square;

  color = SDL_MapRGB (surface->format, r, g, b);

  square.w = squarew;
  square.h = squareh;
  square.x = squarex;
  square.y = squarey;

  SDL_FillRect (surface, &square, color);
}

/** This function permits you to display an empty square in a surface
* \param surface : The surface you want to draw
* \param squarex : The abscissa of the square
* \param squarey : The ordonnee of the square
* \param squarew : The width of the square
* \param squareh : The height of the square
* \param r : The red variant
* \param g : The green variant
* \param b : The blue variant
*/
void
displayEmptySquare (SDL_Surface * surface, int squarex, int squarey,
                    int squarew, int squareh, int r, int g, int b)
{
  displayFullSquare (surface, squarex, squarey, squarew, 1, r, g, b);
  displayFullSquare (surface, squarex, squarey, 1, squareh, r, g, b);
  displayFullSquare (surface, squarex + squarew, squarey, 1, squareh, r, g,
                     b);
  displayFullSquare (surface, squarex, squarey + squareh, squarew, 1, r, g,
                     b);
}

/** This function permits you to copy a surface area in another surface at the wished coordinates 
* \param srcS : The surface you want to copy
* \param destS : The surface where you want to copy
* \param srcx : The abscissa of the area
* \param srcy : The ordonnee of the area
* \param srcw : The width of the area
* \param srch : The height of the area
* \param srcx : The abscissa of the new area's position
* \param srcy : The ordonnee of the new area's position
*/
void
copyPicture (SDL_Surface * srcS, SDL_Surface * destS, int srcx, int srcy,
             int srcw, int srch, int destx, int desty)
{
  SDL_Rect src, dest;

  src.x = srcx;
  src.y = srcy;
  src.w = srcw;
  src.h = srch;

  dest.x = destx;
  dest.y = desty;

  SDL_BlitSurface (srcS, &src, destS, &dest);
}

/** This function totally clear a surface.
* \param surface : the surface you want to clear
*/
void
clearSurface (SDL_Surface * surface)
{
  //Clear a surface... by filling it with black
  SDL_FillRect (surface, NULL, SDL_MapRGB (surface->format, 0, 0, 0));
}

/** This function manages the keys events and the mouse events
* Following the key pressed or the mouse button clicked, the event array is updated
*/
int
manageKeys ()
{
  Uint8 *etat;
  int i;

  //Structure  servant aux événements
  SDL_Event event_list;
  SDL_PumpEvents ();

  //Récupére un tableau contenant l'état des touches (enfoncées ou non)
  etat = SDL_GetKeyState (NULL);

  for (i = 0; i < 100; i++)
    event[i] = 0;


  if (etat[SDLK_UP])
    event[EVENT_UP] = 1;
  if (etat[SDLK_DOWN])
    event[EVENT_DOWN] = 1;
  if (etat[SDLK_LEFT])
    event[EVENT_LEFT] = 1;
  if (etat[SDLK_RIGHT])
    event[EVENT_RIGHT] = 1;
  if (etat[SDLK_LSHIFT] || etat[SDLK_c])
    event[EVENT_CABLE] = 1;
  if (etat[SDLK_SPACE])
    event[EVENT_TIR] = 1;

  //Joystick states
  if (SDL_JoystickGetButton (joy, 0))
    event[EVENT_TIR] = 1;
  if (SDL_JoystickGetButton (joy, 1))
    event[EVENT_CABLE] = 1;

  int value = SDL_JoystickGetAxis (joy, 0);
  //fprintf(stdout, "axis 0 : %d \n", value);
  if (value < -2000)
    event[EVENT_LEFT] = 1;
  if (value > 2000)
    event[EVENT_RIGHT] = 1;

  value = SDL_JoystickGetAxis (joy, 1);
  if (value < -2000)
    event[EVENT_UP] = 1;
  if (value > 2000)
    event[EVENT_DOWN] = 1;


  //Gestion des événements
  while (SDL_PollEvent (&event_list))
    {
      switch (event_list.type)
        {
          //Si appel à quitter
        case SDL_QUIT:
          event[EVENT_QUIT] = 1;
          break;

          //si touche enfoncée
        case SDL_KEYDOWN:
          switch (event_list.key.keysym.sym)
            {
            case SDLK_ESCAPE:
              event[EVENT_QUIT] = 1;
              break;

            case SDLK_RETURN:
              event[EVENT_ENTER] = 1;
              break;

            case SDLK_PAGEUP:
              event[EVENT_NEXTMAP] = 1;
              break;

            case SDLK_PAGEDOWN:
              event[EVENT_PRECMAP] = 1;
              break;

            case SDLK_LEFT:
              event[EVENT_PRESSEDLEFT] = 1;
              break;

            case SDLK_RIGHT:
              event[EVENT_PRESSEDRIGHT] = 1;
              break;

            case SDLK_UP:
              event[EVENT_PRESSEDUP] = 1;
              break;

            case SDLK_DOWN:
              event[EVENT_PRESSEDDOWN] = 1;
              break;

            case SDLK_SPACE:
              event[EVENT_PRESSEDTIR] = 1;
              break;

            case SDLK_LSHIFT:
              event[EVENT_PRESSEDCABLE] = 1;
              break;

            default:
              break;
            }

          switch (event_list.key.keysym.unicode)
            {
            case SDLK_c:
              event[EVENT_PRESSEDCABLE] = 1;
              break;

            default:
              break;

            }
          break;

        case SDL_MOUSEBUTTONDOWN:
          event[EVENT_CLIC] = 1;
          mouse.clicX = (int) event_list.motion.x;
          mouse.clicY = (int) event_list.motion.y;
          switch (event_list.button.button)
            {
            case SDL_BUTTON_LEFT:
              mouse.leftClic = 1;
              break;

            case SDL_BUTTON_RIGHT:
              mouse.rightClic = 1;
              break;
            }
        case SDL_MOUSEMOTION:
          mouse.oldMotionX = mouse.motionX;
          mouse.oldMotionY = mouse.motionY;
          mouse.motionX = (int) event_list.motion.x;
          mouse.motionY = (int) event_list.motion.y;
          event[EVENT_MOTION] = 1;
          break;
        case SDL_MOUSEBUTTONUP:
          event[EVENT_CLIC] = 0;
          mouse.leftClic = mouse.rightClic = 0;
          break;

        case SDL_JOYBUTTONDOWN:
          if (event_list.jbutton.state == SDL_PRESSED)
            {
              if (event_list.jbutton.button == 0)
                event[EVENT_PRESSEDTIR] = 1;
              if (event_list.jbutton.button == 1)
                event[EVENT_PRESSEDCABLE] = 1;
              event[EVENT_ENTER] = 1;
            }
          break;

        case SDL_JOYAXISMOTION:
          /*   if (event_list.jaxis.axis==0)
             {
             if (event_list.jaxis.value>0)
             event[EVENT_PRESSEDRIGHT]=1;
             if (event_list.jaxis.value<0)
             event[EVENT_PRESSEDLEFT]=1;
             }
             if (event_list.jaxis.axis==1)
             {
             if (event_list.jaxis.value>0)
             event[EVENT_PRESSEDDOWN]=1;
             if (event_list.jaxis.value<0)
             event[EVENT_PRESSEDUP]=1;
             } */
          break;
        }
    }

  return 0;
}

/** This function permits to wait for a key press. 
* If a key is pressed, returns 1, else returns 0.
*/
int
keyPressed ()
{
  SDL_Event event;

  while (SDL_PollEvent (&event))
    {
      switch (event.type)
        {
          //si touche enfoncée
        case SDL_KEYDOWN:
          return 1;
          break;

        case SDL_JOYBUTTONDOWN:
          return 1;
          break;
        }
    }

  return 0;
}

/** This function permits you to get the color of a chosen pixel in a surface
* \param surface : the surface you want to get infos
* \param x : the abscissa of the pixel you want to get infos
* \param y : the ordonnee of the pixel you want to get infos
*/
Uint32
getpixel (SDL_Surface * surface, int x, int y)
{
  int bpp = surface->format->BytesPerPixel;

  if (SDL_MUSTLOCK (surface))
    {
      if (SDL_LockSurface (surface) < 0)
        {
          fprintf (stderr, "Can't lock surface: %s\n", SDL_GetError ());
          exit (0);
        }
    }

  Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;


  if (SDL_MUSTLOCK (surface))
    {
      SDL_UnlockSurface (surface);
    }

  switch (bpp)
    {
    case 1:
      return *p;

    case 2:
      return *(Uint16 *) p;

    case 3:
      if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        return p[0] << 16 | p[1] << 8 | p[2];
      else
        return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
      return *(Uint32 *) p;

    default:
      return 0;                 /* shouldn't happen, but avoids warnings */
    }
}

void
putpixel (SDL_Surface * surface, int x, int y, Uint32 pixel)
{
  int bpp = surface->format->BytesPerPixel;

  if (SDL_MUSTLOCK (surface))
    SDL_LockSurface (surface);


  /* Here p is the address to the pixel we want to set */
  Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;

  switch (bpp)
    {
    case 1:
      *p = pixel;
      break;

    case 2:
      *(Uint16 *) p = pixel;
      break;

    case 3:
      if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        {
          p[0] = (pixel >> 16) & 0xff;
          p[1] = (pixel >> 8) & 0xff;
          p[2] = pixel & 0xff;
        }
      else
        {
          p[0] = pixel & 0xff;
          p[1] = (pixel >> 8) & 0xff;
          p[2] = (pixel >> 16) & 0xff;
        }
      break;

    case 4:
      *(Uint32 *) p = pixel;
      break;
    }

  if (SDL_MUSTLOCK (surface))
    SDL_UnlockSurface (surface);

}

SDL_Surface *
displayMask (SDL_Surface * sprite)
{
  int i, j;
  SDL_Surface *tmp;

  tmp = SDL_DisplayFormat (sprite);

  for (i = 0; i < sprite->w; i++)
    {
      for (j = 0; j < sprite->h; j++)
        {
          if (getpixel (sprite, i, j) != sprite->format->colorkey)
            putpixel (tmp, i, j, SDL_MapRGB (sprite->format, 255, 255, 255));
        }
    }

  return tmp;
}

/*! Displays a line between two points 
* \param surface : the surface which will be used
* \param x1 : the abscissa of the first point
* \param y1 : the ordonnee of the first point
* \param x2 : the abscissa of the second point
* \param y2 : the ordonnee of the second point
* \param r : the red variant
* \param g : the green variant
* \param b : the blue variant
*/
void
displayLine (SDL_Surface * surface, int x1, int y1, int x2, int y2, int r,
             int g, int b)
{
  float i, j, k, dx, dy;
  float hyp;                    //int debx, deby, finx, finy;

  hyp = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
  hyp = sqrt (hyp);
  dx = (x2 - x1) / hyp;
  dy = (y2 - y1) / hyp;

  for (i = 0, j = x1, k = y1; i < hyp; i++, j += dx, k += dy)
    {
      putpixel (surface, (int) j, (int) k,
                SDL_MapRGB (surface->format, r, g, b));
}}
