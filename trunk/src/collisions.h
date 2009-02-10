#ifndef _collisions_h
#define _collisions_h

/**************************************************
                FUNCTIONS DECLARATIONS
**************************************************/

/* ! This function return 1 if there is a collision between a poSint32 and a square and 0 if not. */
/*!
  \param x1 the abscissa of the point.
  \param y1 the ordonnee of the point.
  \param x2 the abscissa of the square.
  \param y2 the ordonnee of the square.
  \param w the width of  the square.
  \param h the height of the square.
*/
Sint32 collisionPointSquare(Sint32 x1, Sint32 y1, Sint32 x2, Sint32 y2, Sint32 w, Sint32 h);

/* ! This function return 1 if there is a collision between 2 squares and 0 if not. */
/*!
  \param x1 the abscissa of the 1st square.
  \param y1 the ordonnee of the 1st square.
  \param w1 the width of the 1st square.
  \param h1 the height of the 1st square.
  \param x2 the abscissa of the 2nd square.
  \param y2 the ordonnee of the 2nd square.
  \param w2 the width of the 2nd square.
  \param h2 the height of the 2nd square.
*/
Sint32 collisionSquareSquare(Sint32 x1, Sint32 y1, Sint32 w1, Sint32 h1, Sint32 x2, Sint32 y2, Sint32 w2, Sint32 h2);

/* ! This function return 1 if there is a collision between 2 squares and 0 if not. */
/*!
  \param x1 the abscissa of the 1st square.
  \param y1 the ordonnee of the 1st square.
  \param w1 the width of the 1st square.
  \param h1 the height of the 1st square.
  \param x2 the abscissa of the 2nd square.
  \param y2 the ordonnee of the 2nd square.
  \param w2 the width of the 2nd square.
  \param h2 the height of the 2nd square.
*/
Sint32 testCollisions(Sint32 x1, Sint32 y1, Sint32 w1, Sint32 h1, Sint32 x2, Sint32 y2, Sint32 w2, Sint32 h2);

/* ! This function updates the dx/dy values of an ennemy shoot, with the Bresenham algo. */
/*!
* \param Speed : The speed of the shoot
* \param nbShoot : the number of the shoot
*/
Sint32 manageBresenhamShoots(Sint32 Speed, Sint32 nbShoot);

Sint32 collision(Sint32 x, Sint32 y);

Sint32 manageCollisions(Sint32 type, Sint32 cote, Sint32 x, Sint32 y, Sint32 w, Sint32 h);

void testDoors (void);

void testMortal (void);

Sint32 getEnnemySpoingCollisions(Sint32 cote);

Sint32 ennemyCollisions(Sint32 cote);

Sint32 testRopeCollision (void);

/*! this function tests if a shoot is in collision
* It returns 1 if there is a collision with the game board
* It returns 2 if there is a collision with the player
* It returns 0 if there is no collision
* \param x the abscissa of the shoot
* \param y the ordonnee of the shoot 
*/
Sint32 testShootCollisions(Sint32 x, Sint32 y);

Sint32 detectCollision(SDL_Surface *sprite1, Sint32 x1, Sint32 y1, SDL_Surface *sprite2, Sint32 x2, Sint32 y2);

#endif

