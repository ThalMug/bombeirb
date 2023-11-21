/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef PLAYER_H_
#define PLAYER_H_

// #include <map.h>
#include <constant.h>
#include <bomb.h>

typedef struct player player;
typedef struct map map;
typedef struct game game;
typedef unsigned int uint32_t;

// Creates a new player with a given number of available bombs
struct player* player_init(game* g, int bombs, uint32_t lives, int range) ;
void player_free(struct player* player);

// Get/set player lives count
void player_set_lives(player*, uint32_t lives);
uint32_t player_get_lives(player*);
void player_hit(player* p, int damage);

// Key handling
int player_get_key_state(player*);
void player_set_key_state(player*, int);

// Set the position of the player
void player_set_position(struct player *player, int x, int y);

// Returns the current position of the player
int player_get_x(struct player* player);
int player_get_y(struct player* player);

// Set the direction of the next move of the player
void player_set_current_way(struct player * player, enum direction direction);

// Set, Increase, Decrease the number of bomb that player can put
int  player_get_nb_bomb(struct player * player);
void player_inc_nb_bomb(struct player * player);
void player_dec_nb_bomb(struct player * player);

void bonus_effect(struct player * player,unsigned char type);

// Move the player according to the current direction
int player_move(struct player* player,struct map* map);

// checks if the player should get damage
int player_should_get_hit(player* player, struct map* map);
void player_init_damage_timer(player* player, map* map,uint32_t delta);
uint32_t damage_timer_callback(uint32_t delta, void* player);

// Display the player on the screen
void player_display(struct player* player);

void player_range_dec(struct player * player);
void player_range_inc(struct player * player);

int player_get_range(struct player * player);

#endif /* PLAYER_H_ */
