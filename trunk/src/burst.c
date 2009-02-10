/**
 * @file burst.c 
 * @brief 
 * @created 2007-12-13 
 * @date 2007-12-13
 */
/*
 * copyright (c) 1998-2007 TLK Games all rights reserved
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
int nbStars;

void manageBursts()
{
    int i;
    
    for (i=0; i<NB_BURSTS; i++)
        if (burst[i].life>=0)
           burst[i].life--;            
    for (i=0; i<NB_BIGBURSTS; i++)
        if (bigburst[i].life>=0)
           bigburst[i].life--;            
}

void displayBursts()
{
    int i;
    
    for (i=0; i<NB_BURSTS; i++)
        if (burst[i].life>0)
           copyPicture(IMG_spoing, screen, 775+burst[i].life/(LIFE_BURST/8)*25, 0, 25, 32, burst[i].x - (i%2==1 ? 8 : 0), burst[i].y-16);
    for (i=0; i<NB_BIGBURSTS; i++)
        if (bigburst[i].life>0)
           copyPicture(IMG_spoing, screen, 550+bigburst[i].life/(LIFE_BIGBURST/8)*50, 255, 50, 64, bigburst[i].x , bigburst[i].y); /* - (i%2==1 ? 8 : 0) */
}

void addBurst(int x, int y)
{
    int i;
    
    i=0;
    while (burst[i].life>0 && i<NB_BURSTS)
        i++;
    
    if (i<NB_BURSTS)
    {
        burst[i].x=x;
        burst[i].y=y;
        burst[i].life=LIFE_BURST;
        playSound(3);
    }
}

void addBigBurst(int x, int y)
{
    int i;
    
    i=0;
    while (bigburst[i].life>0 && i<NB_BIGBURSTS)
        i++;
    
    if (i<NB_BIGBURSTS)
    {
        bigburst[i].x=x;
        bigburst[i].y=y;
        bigburst[i].life=LIFE_BIGBURST;
        playSound(2);
    }
}

void initStar()
{
	unsigned i;
  	
    for (i=0; i<NB_STARS; i++)
	   stars[i].life=-1;
   nbStars=0;
}

void addStars()
{
    nbStars=STARS;
}

void manageStars()
{
    static int tps=0;
    int i;
    
    tps++;
    
    if (tps==5)
    {
        tps=0;
        
        if (nbStars>0)
        {
           addStar((int)player.x-player.w/2+ rand()%(player.w*2), (int)player.y-player.h/2 + rand()%(player.h*2));
        }
    }
    
  	for (i=0; i<NB_STARS; i++)
	{ 
        if (stars[i].life!=-1)
            stars[i].life--;
    }   
}

void addStar(int x, int y)
{
	unsigned i;
	i=0;
	
	while(stars[i].life!=-1 && i<NB_STARS) 
       i++;

	if (i<NB_STARS) 
    {
	    stars[i].x=x;
        stars[i].y=y;
    	stars[i].life=LIFE_STAR;
        nbStars--;
   	}
}

void displayStar()
{	
    unsigned i;
    
	for (i=0; i<NB_STARS; i++)
	{ 
        if (stars[i].life!=-1)
		{	
            copyPicture(IMG_spoing, screen, 250+(13*stars[i].life/LIFE_STAR*8), 304, 13, 16, stars[i].x, stars[i].y);
         }    
	}
}

void displayCollapsingBlocks()
{
    int i;
    
    for (i=0;i<NB_COLLAPSINGBLOCK; i++)
    {
        if (collapsingBlock[i].life>0)
           copyPicture(IMG_map, IMG_decor, 450+(5*(LIFE_COLLAPSINGBLOCK-collapsingBlock[i].life)/LIFE_COLLAPSINGBLOCK)*width, 0, width, height, collapsingBlock[i].x, collapsingBlock[i].y);
    }
    
}

void addCollapsingBlock(int x, int y)
{
    int i;
    
    i=0;
    while (collapsingBlock[i].life>0 && i<NB_COLLAPSINGBLOCK)
        i++;
    
    if (i<NB_COLLAPSINGBLOCK)
    {
        collapsingBlock[i].x=x;
        collapsingBlock[i].y=y;
        collapsingBlock[i].life=LIFE_COLLAPSINGBLOCK;
    }
}

void loadCollapsingBlocks()
{
    int i, j;
    
    for (i=0;i<NB_COLLAPSINGBLOCK;i++)
       collapsingBlock[i].life=-1;
       
    for(i=0;i<column;i++)
        for(j=0;j<line;j++)
        {
            if (Col[j*column+i]==COL_EFFRITE)
               addCollapsingBlock(i*width, j*height);   
        }
}

void collapseCollapsingBlock(int x, int y)
{
    int i;
    
    for (i=0;i<NB_COLLAPSINGBLOCK;i++)
    {
        if (collapsingBlock[i].life>0)
           if (collapsingBlock[i].x==x && collapsingBlock[i].y==y)
           {
              collapsingBlock[i].life--;
              if (collapsingBlock[i].life==0)
              {
                 Col[collapsingBlock[i].y/height*column + collapsingBlock[i].x/width]=COL_VIDE;
                 displayShadow(IMG_decor, collapsingBlock[i].x/width, collapsingBlock[i].y/height);
              }
           }
    }
}

void manageCollapsingBlocks()
{
    int i, j;
    
    for (i=((int)player.x-2+(player.dx<0 ? (int)player.dx : 0))/width*width;i<=((int)player.x+player.w+2+(player.dx>0 ? (int)player.dx : 0))/width*width;i+=width)
       for (j=((int)player.y-2+(player.dy<0 ? (int)player.dy : 0))/height*height; j<=((int)player.y+player.h+2+(player.dy>0 ? (int)player.dy : 0))/height*height; j+=height)
          if (getCollision(i, j)==COL_EFFRITE)
             collapseCollapsingBlock(i, j); 
    for (i=0; i<NB_COLLAPSINGBLOCK; i++)
       if (collapsingBlock[i].life<LIFE_COLLAPSINGBLOCK)
          collapseCollapsingBlock(collapsingBlock[i].x, collapsingBlock[i].y); 
}

