/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef MAP_H_
#define MAP_H_

#include "constant.h"
#include "Vector2.h"
#include <stdio.h>

typedef struct player player;
typedef struct game game;

typedef enum cell_type {
	CELL_EMPTY=0x00,			//  0000 0000
	CELL_PLAYER_SPAWN=0x0f,		//  0000 1111
	CELL_SCENERY=0x10,			//  0001 0000
	CELL_BOX=0x20,				//  0010 0000
	CELL_DOOR=0x30,				//  0011 0000
	CELL_KEY=0x40,				//  0100 0000
	CELL_BONUS=0x50,			// 	0101 0000
	CELL_MONSTER=0x60,			// 	0110 0000
	CELL_BOMB=0x70,				// 	0111 0000
	CELL_EXPLOSION=0x80,		//  1000 0000
}cell_t;

enum bonus_type {
	BONUS_BOMB_RANGE_DEC=1,
	BONUS_BOMB_RANGE_INC,
	BONUS_BOMB_NB_DEC,
	BONUS_BOMB_NB_INC,
	BONUS_MONSTER,
	BONUS_LIFE
};
enum scenery_type {
	SCENERY_STONE = 1,    // 0001 
	SCENERY_TREE  = 2,    // 0010 
	SCENERY_PRINCESS = 4  // 0100 
};

enum compose_type {
	CELL_TREE     = CELL_SCENERY | SCENERY_TREE,
	CELL_STONE    = CELL_SCENERY | SCENERY_STONE,
	CELL_PRINCESS = CELL_SCENERY | SCENERY_PRINCESS,

    CELL_BOX_RANGEINC = CELL_BOX | BONUS_BOMB_RANGE_DEC,
    CELL_BOX_RANGEDEC = CELL_BOX | BONUS_BOMB_RANGE_INC,
	CELL_BOX_BOMBINC  = CELL_BOX | BONUS_BOMB_NB_DEC,
    CELL_BOX_BOMBDEC  = CELL_BOX | BONUS_BOMB_NB_INC,
    CELL_BOX_LIFE     = CELL_BOX | BONUS_MONSTER,
    CELL_BOX_MONSTER  = CELL_BOX | BONUS_LIFE,

	CELL_DOOR_OPEN		= CELL_DOOR | 1,	// 0011 0001
	CELL_DOOR_CLOSED	= CELL_DOOR,		// 0011 0000
};

typedef struct map map;

// Create a new empty map
struct map* map_new(game* g, int width, int height);
void map_free(struct map* map);


// Spawning monsters
void map_spawn_monster(map* map, int x, int y);

void map_set_player(map*, player*);
struct player* map_get_player(map* map);

unsigned char* map_get_grid(map*);
// void map_set_grid(map*, unsigned char* grid);

// Return the height and width of a map
int map_get_width(struct map* map);
int map_get_height(struct map* map);

// Return the type of a cell
enum cell_type map_get_cell_type(struct map* map, int x, int y);

// Set the type of a cell
void  map_set_cell_type(struct map* map, int x, int y, enum cell_type type);

// Tries to move a cell
int map_cell_move(struct map* map, int x, int y, Vector2 d);

// Test if (x,y) is within the map
int map_is_inside(struct map* map, int x, int y);

// player spawn point handling
Vector2 map_get_player_spawn(map*);
void map_set_player_spawn(map*, int x, int y);

// load all maps
int map_load_all(map** output, game* g);

// Display the map on the screen
void map_display(struct map* map);


void display_bonus(struct map* map, int x, int y, unsigned char type);

#endif /* MAP_H_ */
