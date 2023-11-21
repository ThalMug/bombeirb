/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <assert.h>
#include <time.h>

#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <bomb.h>
#include <errno.h>


#define SAVE_FILE_PATH "../saves/save.s"

struct game {
	struct map** maps;       			// the game's map
	short levels;        				// nb maps of the game
	short level;						// current map
	unsigned char onPause;				// pause state
	unsigned int winWidth, winHeight;	// window size
	struct player* player;
};

struct game* game_new(void) {
	sprite_load(); // load sprites into process memory

	struct game* game = malloc(sizeof(game));
	game->onPause = 0;
	game->maps = calloc(MAX_LEVELS_COUNT, sizeof(map*));
	game->levels = 0;
	game->player = player_init(game, 3, 7, 2);
	game->levels = map_load_all(game->maps, game);
	// check if at least one map as been created
	assert(game->levels);
	game->winWidth = map_get_width(game->maps[0]) * SIZE_BLOC;
	game->winHeight = map_get_width(game->maps[0]) * SIZE_BLOC + 
		BANNER_HEIGHT + LINE_HEIGHT;
	Vector2 spawn = map_get_player_spawn(game->maps[0]);
	player_set_position(game->player, spawn.x, spawn.y);
	game->level = 0;

	map_set_player(game->maps[0], game->player);
	// Set default location of the player

	return game;
}
unsigned int game_get_window_width(game* g)
{
	assert(g); return g->winWidth;
}
unsigned int game_get_window_height(game* g)
{
	assert(g); return g->winHeight;
}
// switch to the next/previous map
void game_change_level(struct game* game, int offset)
{
	assert(game);
	if(!IS_BETWEEN(game->level+offset, 0, game->levels-1)) return;
	game->level += offset;
	map* new_map = game->maps[game->level];
	Vector2 spawn = map_get_player_spawn(new_map);
	player_set_position(game->player, spawn.x, spawn.y);
	player_set_key_state(game->player, 0);
}
// save the state of the game under ../saves/save.s
size_t save_game(game* game)
{
	FILE* saveFile = fopen(SAVE_FILE_PATH, "wb");
	size_t size = 0;
	if(!saveFile)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	size += fwrite("game", 1, 4, saveFile);
	unsigned char gameData[3] = {
		game->levels,
		game->level,
		game->onPause,
	};
	size += fwrite(gameData, 1, 3, saveFile);
	player* p = game->player;
	unsigned char playerData[6] = {
		player_get_x(p),
		player_get_y(p),
		player_get_lives(p),
		player_get_nb_bomb(p),
		player_get_key_state(p),
		player_get_range(p)
	};
	size += fwrite("player", 1, 6, saveFile);
	size += fwrite(playerData, 1, 6, saveFile);
	size += fwrite("bombs", 1, 5, saveFile);
	unsigned char bombCount = bomb_get_count();
	size += fwrite(&bombCount, 1, 1, saveFile);
	bomb** bombs = get_bombs();
	for (int i = 0; i < bombCount; i++)
	{
		bomb_data data = bomb_get_data(bombs[i]);
		unsigned char buffer[6] = {
			data.x, data.y,
			data.timer&0xff, (data.timer<<8)&0xff, 
			(data.timer<<16)&0xff,
			(data.timer<<24)&0xff
		};
		size += fwrite(buffer, 1, 6, saveFile);
	}
	size += fwrite("maps", 1, 4, saveFile);
	for (short i = 0; i < game->levels; i++)
	{
		map* m = game->maps[i];
		unsigned char mapData[4] = {
			map_get_width(m),
			map_get_height(m),
			map_get_player_spawn(m).x,
			map_get_player_spawn(m).y,
		};
		size += fwrite(mapData, 1, 4, saveFile);
		size += fwrite(map_get_grid(m), 1, mapData[0] * mapData[1], saveFile);
	}
	
	fclose(saveFile);
	return size;
}
// load the data from the save.s file or returns null if the file doesn't exist
game* save_load()
{
	typedef unsigned char byte;
	FILE* sFile = fopen(SAVE_FILE_PATH, "rb");
	if(!sFile) return NULL;
	sprite_load(); // load sprites into process memory
	game* game = malloc(sizeof(game));
	byte gameData[3];
	fseek(sFile, 4, SEEK_CUR);
	// read global game data
	fread(gameData, 1, 3, sFile);
	game->levels = gameData[0];
	game->level = gameData[1];
	game->onPause = gameData[2];
	game->maps = malloc(game->levels * sizeof(map*));
	fseek(sFile, 6, SEEK_CUR);
	byte playerData[6];
	// read player data
	size_t n = fread(playerData, 1, 6, sFile);
	if(n<5) { free(game); return NULL; } // if player data was incomplete
	game->player = player_init(game, playerData[3], playerData[2], playerData[5]);
	player_set_key_state(game->player, playerData[4]);
	player_set_position(game->player, playerData[0], playerData[1]);
	// read bomb data
	fseek(sFile, 5, SEEK_CUR);
	unsigned char bombCount;
	fread(&bombCount, 1, 1, sFile);
	bomb** bombs = get_bombs();
	for (int i = 0; i < bombCount; i++)
	{
		unsigned char data[6];
		fread(data, 1, 6, sFile);
		bombs[i] = bomb_init(data[0], data[1]);
		bomb_set_game(bombs[i], game);
		int timer = LITLE_ENDIAN((data+2));
		bomb_set_type(bombs[i], (bomb_t)(timer/1000));
		bomb_set_timer(bombs[i], LITLE_ENDIAN((data+2)));
	}
	// read maps
	fseek(sFile, 4, SEEK_CUR);
	for (short i = 0; i < game->levels; i++)
	{
		byte mapData[4];
		n = fread(mapData, 1, 4, sFile);
		if(n<4) { free(game); return NULL; }
		map* m = map_new(game, mapData[0], mapData[1]);
		byte* grid = map_get_grid(m);
		// read the grid
		n = fread(grid, 1, mapData[0] * mapData[1], sFile);
		if(n != (mapData[0] * mapData[1]))
		{
			map_free(m); free(game); return NULL;
		}
		// spawning the monsters
		for (int y = 0; y < mapData[1]; y++)
		{
			for (int x = 0; x < mapData[0]; x++)
			{
				if((grid[mapData[0]*y + x]&0xf0) == CELL_MONSTER)
					map_spawn_monster(m, x, y);
			}
		}
		map_set_player(m, game->player);
		map_set_player_spawn(m, mapData[2], mapData[3]);
		game->maps[i] = m;
	}
	game->winWidth = map_get_width(game->maps[0]) * SIZE_BLOC;
	game->winHeight = map_get_width(game->maps[0]) * SIZE_BLOC + 
		BANNER_HEIGHT + LINE_HEIGHT;

	for(int i=0; bombs[i]; i++)
		bomb_set_map(bombs[i], game->maps[game->level]);
	return game;
}
unsigned char game_is_on_pause(struct game* g)
{
	assert(g); return g->onPause;
}
void game_set_pause_state(game* g, int state)
{
	assert(g); g->onPause = state? 1:0;
}

void game_free(struct game* game) {
	assert(game);

	player_free(game->player);
	for (int i = 0; i < game->levels; i++)
		map_free(game->maps[i]);
	sprite_free();
}

struct map* game_get_current_map(struct game* game) {
	assert(game);
	return game->maps[game->level];
}


struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;
}

void game_banner_display(struct game* game) {
	assert(game);

	struct map* map = game_get_current_map(game);

	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 6 * SIZE_BLOC) / 4;
	int x = white_bloc;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), x, y);

	x = white_bloc + SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_lives(game->player)), x, y);

	x = 2 * white_bloc + 2 * SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x = 2 * white_bloc + 3 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

	x = 3 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 3 * white_bloc + 5 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_range(game->player)), x, y);
}

void game_display(struct game* game) {
	assert(game);

	window_clear();
	game_banner_display(game);
	map_display(game_get_current_map(game));
	player_display(game->player);


	window_refresh();
}


static short input_keyboard(struct game* game) {
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct map* map = game_get_current_map(game);
	int x = player_get_x(player), y = player_get_y(player);
	cell_t cell = map_get_cell_type(map, x, y);
	if(game->onPause)
	{
		while(SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				return 1;
				break;
			case SDL_KEYDOWN:
				game->onPause ^= (event.key.keysym.sym==SDLK_p
					|| event.key.keysym.sym == SDLK_ESCAPE);
			default:
				break;
			}
		}
		return 0;
	}
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_UP:
				player_set_current_way(player, NORTH);
				player_move(player, map);
				break;
			case SDLK_DOWN:
				player_set_current_way(player, SOUTH);
				player_move(player, map);
				break;
			case SDLK_RIGHT:
				player_set_current_way(player, EAST);
				player_move(player, map);
				break;
			case SDLK_LEFT:
				player_set_current_way(player, WEST);
				player_move(player, map);
				break;
			case SDLK_SPACE: ;
				if(player_get_nb_bomb(player) != 0){
					player_dec_nb_bomb(player);
					bomb_tab_add(player_get_x(player),player_get_y(player),map, game);
					map_set_cell_type(map,player_get_x(player),player_get_y(player),CELL_BOMB);
					break;
				}		
				break;
			case SDLK_ESCAPE:
			case SDLK_p:
				game->onPause ^= 1;
				break;
			case SDLK_e:
				if(player_get_key_state(player) && (cell&0xfd) == CELL_DOOR_CLOSED)
					map_set_cell_type(map, x, y, CELL_DOOR_OPEN);
				break;
			default:
				break;
			}
			break;
		}
	}
	return 0;
}

int game_update(struct game* game) {
	if (input_keyboard(game))
		return 1; // exit game
	if(!game->onPause)
	{
		int x = player_get_x(game->player), y = player_get_y(game->player);
		cell_t cell = map_get_cell_type(game->maps[game->level], x, y);
		if((cell & 0xfd) == CELL_DOOR_OPEN)
			game_change_level(game, (cell&2)? -1: 1);
	}
	return 0;
}
