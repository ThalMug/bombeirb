/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL.h>

#include <constant.h>
#include <game.h>
#include <window.h>
#include <misc.h>
#include <time.h>
#include <stdlib.h>
#include "rng.h"

int main(int argc, char *argv[]) {

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		error("Can't init SDL:  %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	rng_init((uint32_t)time(NULL));
	struct game* game = save_load();
	if(!game) game = game_new();

	window_create(game_get_window_width(game),
		game_get_window_height(game));

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	// to obtain the DEFAULT_GAME_FPS, we have to reach a loop duration of (1000 / DEFAULT_GAME_FPS) ms
	int ideal_speed = 1000 / DEFAULT_GAME_FPS;
	int timer, execution_speed;

	// game loop
	// static time rate implementation
	int done = 0;
	while (!done) {
		timer = SDL_GetTicks();

		done = game_update(game);
		game_display(game);

		execution_speed = SDL_GetTicks() - timer;
		if (execution_speed < ideal_speed)
			SDL_Delay(ideal_speed - execution_speed); // we are ahead of ideal time. let's wait.
	}
	fprintf(stdout, "%u\n", execution_speed);
	//save_game(game);
	game_free(game);
	SDL_Quit();

	return EXIT_SUCCESS;
}


