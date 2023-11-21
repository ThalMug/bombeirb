/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef GAME_H_
#define GAME_H_

#include <player.h>
#include <map.h>
#include <stddef.h>

// Abstract data type
typedef struct game game;

// Create a new game
struct game* game_new();


// level changing
void game_change_level(struct game*, int off);

// save handling
size_t save_game(game*);
game* save_load();

unsigned int game_get_window_width(game* g);
unsigned int game_get_window_height(game* g);

// Free a game
void game_free(struct game* game);

// Return the player of the current game
struct player* game_get_player(struct game* game);

// Return the current map
struct map* game_get_current_map(struct game* game);

// Display the game on the screen
void game_display(struct game* game);

// update
int game_update(struct game* game);

unsigned char game_is_on_pause(struct game* g);
void game_set_pause_state(game* g, int state);


#endif /* GAME_H_ */
