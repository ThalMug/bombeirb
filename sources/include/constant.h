/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef CONSTANT_H_
#define CONSTANT_H_

#include <SDL/SDL.h>
#include <assert.h>

#define WINDOW_NAME "[PG110] Bombeirb 2018-2019"

// Size (# of pixels) of a cell of the map
#define SIZE_BLOC       40

// Size (# of pixels) of banner
#define LINE_HEIGHT	4
#define BANNER_HEIGHT	40

#define MAX_LEVELS_COUNT 20

#define MAX_MONSTERS_COUNT 16
#define MONSTER_DAMAGE_DELAY 1000

typedef unsigned int uint32_t;

// Time management
#define DEFAULT_GAME_FPS 30

// map size
#define STATIC_MAP_WIDTH  12
#define STATIC_MAP_HEIGHT 12

typedef enum direction {
	NORTH = 0,
	EAST,  // 1
	SOUTH, // 2
	WEST   // 3
} direction;

#endif /* CONSTANT */
