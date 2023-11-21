/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "rng.h"

#include "monster.h"
#include <map.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>
#include <bomb.h>
#include <game.h>

typedef unsigned char byte;
#define MAP_PATH "../maps/maps.m"
#define MAP_TOTAL_SIZE (STATIC_MAP_WIDTH * 3 * STATIC_MAP_HEIGHT)

typedef struct map {
	unsigned char* grid;
	int width;
	int height;
	Vector2 player_spawn;
	int monsters_count;
	monster* monsters[MAX_MONSTERS_COUNT];
	player* player;
	game* game;
} map;

#define CELL(i,j) ( (i) + (j) * map->width)

// create a new monster
void map_spawn_monster(map* map, int x, int y)
{
	assert((map));
	assert(map->monsters_count < MAX_MONSTERS_COUNT);
	assert(map_is_inside(map, x, y));
	direction d = rng_rand() & 3;
	map->monsters[map->monsters_count] = monster_new(map,map->game, x, y, d);
	map->monsters_count++;
	map->grid[CELL(x, y)] = CELL_MONSTER | d;
}
void map_set_player(map* map, player* player)
{
	assert(map && player);
	map->player = player;
}
player* map_get_player(map* map){ return map->player; }
// create a new empty map
struct map* map_new(game* g, int width, int height)
{
	assert(width > 0 && height > 0);

	struct map* map = malloc(sizeof *map);
	if (!map)
		error("map_new : malloc map failed");
	map->game = g;
	map->width = width;
	map->height = height;
	map->monsters_count = 0;
	map->player = game_get_player(g);
	map->player_spawn.x = map->player_spawn.y = 0;
	memset(map->monsters, 0, MAX_MONSTERS_COUNT * sizeof(monster*));

	map->grid = malloc(height * width);
	if (map->grid == NULL) {
		error("map_new : malloc grid failed");
	}

	// Grid cleaning
	int i, j;
	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++)
			map->grid[CELL(i,j)] = CELL_EMPTY;

	return map;
}

unsigned char* map_get_grid(map* m){ assert(m && m->grid); return m->grid; }
void map_set_grid(map* m, unsigned char* grid){ assert(m && grid); m->grid = grid; }

// checks if (x,y) is inside the map
int map_is_inside(struct map* map, int x, int y) //function used to know if the couple (x,y) is inside the map
{	
	assert(map);
	if((x < map->width) && (y < map->height) && (0 <= y) && (0 <= x)){ 
		return 1;
	}	
	else{
		return 0;
	}
}

Vector2 map_get_player_spawn(map* m){ assert(m); return m->player_spawn; }
void map_set_player_spawn(map* map, int x, int y)
{
	assert(map_is_inside(map, x, y));
	map->player_spawn = (Vector2){x,y};
}

void map_free(struct map *map)
{
	if (map == NULL )
		return;
	free(map->grid); // triggers an exception at the end for some reason, gotta figure this one out 
	for(int i=0; map->monsters[i] && i<MAX_MONSTERS_COUNT; i++)
		monster_free(map->monsters[i]);
	free(map);
}

int map_get_width(struct map* map)
{
	assert(map != NULL);
	return map->width;
}

int map_get_height(struct map* map)
{
	assert(map);
	return map->height;
}

enum cell_type map_get_cell_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)];
}

void map_set_cell_type(struct map* map, int x, int y, cell_t type)
{
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = type;
}
// translate the cell at position (x,y) by d 
int map_cell_move(struct map* map, int x, int y, Vector2 d)
{
	assert(map);
	if(!map_is_inside(map, x+d.x, y+d.y)) return 0;
	else if(map_get_cell_type(map, x+d.x, y+d.y) != CELL_EMPTY)
		return 0;

	map_set_cell_type(map, x+d.x, y+d.y, map_get_cell_type(map, x, y));
	map_set_cell_type(map, x, y, CELL_EMPTY);
	return 1;
}
void display_bonus(struct map* map, int x, int y, unsigned char type)
{
	// bonus is encoded with the 4 most significant bits
	switch (type & 0x0f) {
	case BONUS_BOMB_RANGE_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_INC), x, y);
		break;

	case BONUS_BOMB_RANGE_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_DEC), x, y);
		break;

	case BONUS_BOMB_NB_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_DEC), x, y);
		break;

	case BONUS_BOMB_NB_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_INC), x, y);
		break;
	case BONUS_LIFE:
		window_display_image(sprite_get_banner_life(), x, y);
		break;
	case BONUS_MONSTER:
		//monster_new(map,map->game,x,y,SOUTH);
		break;
	}
}

void display_scenery(struct map* map, int x, int	y, unsigned char type)
{
	switch (type & 0x0f) { // sub-types are encoded with the 4 less significant bits
	case SCENERY_STONE:
		window_display_image(sprite_get_stone(), x, y);
		break;

	case SCENERY_TREE:
		window_display_image(sprite_get_tree(), x, y);
		break;
	}
}



void map_display(struct map* map)
{
	assert(map != NULL);
	assert(map->height > 0 && map->width > 0);

	int x, y;
	for (int i = 0; i < map->width; i++) {
		for (int j = 0; j < map->height; j++) {
			x = i * SIZE_BLOC;
			y = j * SIZE_BLOC;

			unsigned char type = map->grid[CELL(i,j)];	
			
			switch (type & 0xf0) {
		case CELL_SCENERY:
			display_scenery(map, x, y, type);
			break;
			case CELL_BOX:
				window_display_image(sprite_get_box(), x, y);
				break;
			case CELL_BONUS:
				display_bonus(map, x, y, type);
				break;
			case CELL_KEY:
				window_display_image(sprite_get_key(), x, y);
				break;
			case CELL_DOOR:
				if(type & 1) // if the door is open
					window_display_image(sprite_get_door_opened(), x, y);
				else
					window_display_image(sprite_get_door_closed(), x, y);
				break;
			case CELL_MONSTER:
				window_display_image(sprite_get_monster(type & 3), x, y);
				break;
		
			case CELL_BOMB: ;		
		 		struct bomb* bomb = bomb_seek(x,y);
				
		 	 	bomb_display(bomb,x,y,map,map_get_player(map));
				if(bomb_timer(bomb) >= 4 ){
					explosion_display(x,y,map);
					bomb_set_explosion_timer(bomb,1000);
				}
	  	  		break;
			case CELL_EXPLOSION:
				window_display_image(sprite_get_explosion(),x,y);
		}
	  }
	}
}

// reads the maps.m file and loads the data
int map_load_all(map** output, game* g)
{
	FILE* in = fopen(MAP_PATH, "rb");
	assert(in);
	int i=0;
	for(;i < MAX_LEVELS_COUNT; i++)
	{
		unsigned char mapSize[2] = {0};
		int n = fread(mapSize, 1, 2, in);
		if (n != 2)
			return i;

		map* map = map_new(g, mapSize[0], mapSize[1]);
		n = fread(map->grid, 1, map->width*map->height, in);
		assert(n == map->width*map->height);
		for (int j = 0; j < map->height; j++)
		{
			for (int i = 0; i < map->width; i++)
			{
				if((map->grid[CELL(i,j)] & 0xf0) == CELL_MONSTER)
					map_spawn_monster(map, i, j);
				else if(map->grid[CELL(i,j)] == CELL_PLAYER_SPAWN)
				{
					map->player_spawn.x = i;
					map->player_spawn.y = j;
				}
			}
		}
		output[i] = map;
	}
	return 0;
}
