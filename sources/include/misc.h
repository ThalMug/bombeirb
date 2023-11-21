/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef MISC_H_
#define MISC_H_

#include <SDL/SDL.h>

#define LITLE_ENDIAN(b) ((int)(b[0] | (b[1]<<8) | (b[2]<<16) | (b[3]<<24)))
#define IS_BETWEEN(x, low, high) ((x>=low) && (x<=high))

// Print an error message on stderr and exit
void error(const char *s, ...);

int distance(int i, int j,int x, int y);

// Load an image, raise an error in case of failure
SDL_Surface* image_load(const char *filename);

#endif /* MISC_H_ */
