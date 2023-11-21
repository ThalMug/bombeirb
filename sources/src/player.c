/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <SDL/SDL.h>
#include <assert.h>

#include <player.h>
#include "map.h"
#include "game.h"
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <Vector2.h>

struct player {
	int x, y;
	int bombs;
	int hasKey;
	uint32_t lives;
	enum direction direction;
	SDL_TimerID monsterDamageTimer;
	int range;
	game* game;
	// map* current_map;
};

const static Vector2 directions[4] = {
	{0, -1},
	{1,  0},
	{0,  1},
	{-1, 0}
};

struct player* player_init(game* g, int bombs, uint32_t lives, int range) 
{
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");
	player->game = g;
	player->direction = NORTH;
	player->bombs = bombs;
	player->lives = lives;
	player->monsterDamageTimer = NULL;
	player->hasKey = 0;
	player->range = range;

	return player;
}

void player_set_lives(player* p, uint32_t lives)
{
	assert(p);
	p->lives = lives>=0? lives: 0;
}
void player_hit(player* p, int damage)
{
	player_set_lives(p, p->lives - damage);
}
uint32_t player_get_lives(player* p)
{ 
	assert(p && p->lives); 
	return (p->lives>=0)? p->lives: 0; 
}
// checks whether or not the player should take damage
int player_should_get_hit(player* player, struct map* map)
{
	if(!player) return 0;
	return (((map_get_cell_type(map, player->x, player->y) & 0xf0) == CELL_MONSTER) || ((map_get_cell_type(map, player->x, player->y) & 0xf0) == CELL_EXPLOSION));
}
// init the timer when the player meets a monster
void player_init_damage_timer(player* player, map* map, uint32_t delta)
{
	assert(player);
	if(player->monsterDamageTimer) SDL_RemoveTimer(player->monsterDamageTimer);
	player->monsterDamageTimer = SDL_AddTimer(delta, damage_timer_callback, map);
}
// callback when the timer hits 0
Uint32 damage_timer_callback(Uint32 delta, void* map)
{
	player* p = map_get_player(map);
	while(game_is_on_pause(p->game))
		SDL_Delay(TIMER_RESOLUTION);
	if(!player_should_get_hit(p, map))
		return 0;
	
	player_hit(p, 1);
	return MONSTER_DAMAGE_DELAY;
	
}

void player_set_position(struct player *player, int x, int y) {
	assert(player);
	player->x = x;
	player->y = y;
}



void player_free(struct player* player) {
	assert(player);
	if(player->monsterDamageTimer) SDL_RemoveTimer(player->monsterDamageTimer);
	free(player);
}

int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}
// does the player have the key
int player_get_key_state(player* p){ assert(p); return p->hasKey; }
void player_set_key_state(player* p, int state){ assert(p); p->hasKey = state; }

void player_set_current_way(struct player* player, enum direction way) {
	assert(player);
	player->direction = way;
}

int player_get_nb_bomb(struct player* player) {
	assert(player);
	return player->bombs;
}

void player_inc_nb_bomb(struct player* player) {
	assert(player);
	player->bombs += 1;
}

void player_dec_nb_bomb(struct player* player) {
	assert(player);
	player->bombs -= 1;
}

int player_get_range(struct player *player){
	assert(player);
	return player->range;
}

void player_range_inc(struct player * player){
	assert(player);
	player->range++;
}

void player_range_dec(struct player * player){
	assert(player);
	player->range--;
}

void bonus_effect(struct player * player,unsigned char type){
	// bonus is encoded with the 4 least significant bits
	switch (type & 0x0f) {
	case BONUS_BOMB_RANGE_INC:
		player_range_inc(player);
		break;

	case BONUS_BOMB_RANGE_DEC:
		player_range_inc(player);
		break;

	case BONUS_BOMB_NB_DEC:
		player_dec_nb_bomb(player);
		break;

	case BONUS_BOMB_NB_INC:
		player_inc_nb_bomb(player);
		break;
	case BONUS_LIFE:
		player_set_lives(player,player_get_lives(player)+1);
		break;
	case BONUS_MONSTER:
		//map_spawn_monster(map,map->game,x,y,SOUTH);
		break;
	}
}
// checks whether the move is valid or not
static int player_move_aux(struct player* player, struct map* map, Vector2 d) {
	int x = player->x + d.x;
	int y = player->y + d.y;
	
	if (!map_is_inside(map, x, y))
		return 0;
	unsigned char cell = map_get_cell_type(map, x, y);
	switch (cell & 0xf0) {
	case CELL_SCENERY:
		return (cell == CELL_PRINCESS);
		break;

	case CELL_BOX:
		return map_cell_move(map, x, y, d);
		break;
	case CELL_KEY:
		player->hasKey = 1;
		map_set_cell_type(map, x, y, CELL_EMPTY);
		return 1;
	case CELL_BONUS:
		bonus_effect(player,cell);
		map_set_cell_type(map,x,y,CELL_EMPTY);
		break;

	case CELL_MONSTER:
		player_hit(player, 1);
		player->monsterDamageTimer = SDL_AddTimer(MONSTER_DAMAGE_DELAY,
			damage_timer_callback, map);
		break;
	case CELL_EXPLOSION:
		player_hit(player, 1);
		player->monsterDamageTimer = SDL_AddTimer(MONSTER_DAMAGE_DELAY,
			damage_timer_callback, map);
		break;
	case CELL_DOOR:
		// if we're exiting the map, update the spawn point
		map_set_player_spawn(map, player->x, player->y);
		break;
	default:
		break;
	}

	// Player has moved
	return 1;
}

int player_move(struct player* player, struct map* map) {
	// int x = player->x;
	// int y = player->y;
	int move = 0;

	Vector2 d = directions[player->direction];

	if(player_move_aux(player, map, d))
	{
		move = 1;
		player->x += d.x;
		player->y += d.y;
	}

	// if (move) {
	// 	cell_t type = map_get_cell_type(map, x, y);
	// 	map_set_cell_type(map, x, y, 
	// 		(type!=CELL_BOMB && (type&0xf0)!=CELL_MONSTER)? CELL_EMPTY: type);
	// }
	return move;
}

void player_display(struct player* player) {
	assert(player);
	window_display_image(sprite_get_player(player->direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}

