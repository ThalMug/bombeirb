/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h> // IMG_Load
#include <misc.h>

void error(const char *s, ...) {
	va_list ap;
	va_start(ap, s);

	vfprintf(stderr, s, ap);
	va_end(ap);
	exit(EXIT_FAILURE);
}

int distance(int i, int j,int x, int y){
	int x_squared = (i-x)*(i-x);
	int y_squared = (j-y)*(j-y);
	int sum = x_squared+y_squared;
	if(sum < 2){
		return 1;
	}
	else{
		return 0;
	}

}

SDL_Surface* image_load(const char* filename) {
	SDL_Surface* img = IMG_Load(filename);
	if (!img)
		error("IMG_Load: %s\n", IMG_GetError());

	return img;
}

