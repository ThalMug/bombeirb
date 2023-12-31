/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>

#include <sprite.h>
#include <misc.h>


// Sprites general
#define MAP_CASE        "sprite/wood_box.png"
#define MAP_KEY			"sprite/key.png"
#define MAP_DOOR_OPENED	"sprite/door_opened.png"
#define MAP_DOOR_CLOSED	"sprite/door_closed.png"


// Scenery elements
#define MAP_STONE		"sprite/stone.png"
#define MAP_TREE        "sprite/tree.png"
#define MAP_GOAL		"sprite/flag.png"

// Sprites of Banner
#define BANNER_LINE		"sprite/banner_line.png"
#define BANNER_LIFE		"sprite/banner_life.png"
#define BANNER_BOMB		"sprite/bomb3.png"
#define BANNER_RANGE	"sprite/banner_range.png"
#define DIGIT_0			"sprite/banner_0.jpg"
#define DIGIT_1			"sprite/banner_1.jpg"
#define DIGIT_2			"sprite/banner_2.jpg"
#define DIGIT_3			"sprite/banner_3.jpg"
#define DIGIT_4			"sprite/banner_4.jpg"
#define DIGIT_5			"sprite/banner_5.jpg"
#define DIGIT_6			"sprite/banner_6.jpg"
#define DIGIT_7			"sprite/banner_7.jpg"
#define DIGIT_8			"sprite/banner_8.jpg"
#define DIGIT_9			"sprite/banner_9.jpg"

#define PAUSE_TITLE		"sprite/pause_title.png"
#define PAUSE_RESUME	"sprite/pause_resume.png"
#define PAUSE_SAVE		"sprite/pause_save.png"
#define PAUSE_QUIT		"sprite/pause_quit.png"

// Sprites of Bombs
#define BOMB_TTL1       "sprite/bomb1.png"
#define BOMB_TTL2       "sprite/bomb2.png"
#define BOMB_TTL3       "sprite/bomb3.png"
#define BOMB_TTL4       "sprite/bomb4.png"

//sprite of Explosion

#define EXPLOSION		"sprite/explosion.png"

// Sprites of Bonus
#define IMG_BONUS_BOMB_RANGE_INC  "sprite/bonus_bomb_range_inc.png"
#define IMG_BONUS_BOMB_RANGE_DEC  "sprite/bonus_bomb_range_dec.png"
#define IMG_BONUS_BOMB_NB_INC     "sprite/bonus_bomb_nb_inc.png"
#define IMG_BONUS_BOMB_NB_DEC     "sprite/bonus_bomb_nb_dec.png"

// Sprites of Players
#define PLAYER_LEFT     "sprite/player_left.png"
#define PLAYER_UP       "sprite/player_up.png"
#define PLAYER_RIGHT    "sprite/player_right.png"
#define PLAYER_DOWN     "sprite/player_down.png"

#define MONSTER_LEFT	"sprite/monster_left.png"
#define MONSTER_UP		"sprite/monster_up.png"
#define MONSTER_RIGHT	"sprite/monster_right.png"
#define MONSTER_DOWN	"sprite/monster_down.png"

// banner
SDL_Surface* numbers[10];
SDL_Surface* banner_life;
SDL_Surface* banner_bomb;
SDL_Surface* banner_range;
SDL_Surface* banner_line;

// pause menu
SDL_Surface* pause_menu_img[4] = {NULL};

// map
SDL_Surface* box;
SDL_Surface* key;
SDL_Surface* door_opened;
SDL_Surface* door_closed;
SDL_Surface* stone;
SDL_Surface* tree;

// bonus
#define NB_BONUS 4
SDL_Surface* bonus[NB_BONUS + 1];

// player
SDL_Surface* player_img[4];

// monster
SDL_Surface* monster_img[4];

// bombs
SDL_Surface* bomb1;
SDL_Surface* bomb2;
SDL_Surface* bomb3;
SDL_Surface* bomb4;
SDL_Surface* explosion;

static void banner_load() {
	// numbers imgs
	numbers[0] = image_load(DIGIT_0);
	numbers[1] = image_load(DIGIT_1);
	numbers[2] = image_load(DIGIT_2);
	numbers[3] = image_load(DIGIT_3);
	numbers[4] = image_load(DIGIT_4);
	numbers[5] = image_load(DIGIT_5);
	numbers[6] = image_load(DIGIT_6);
	numbers[7] = image_load(DIGIT_7);
	numbers[8] = image_load(DIGIT_8);
	numbers[9] = image_load(DIGIT_9);

	// other banner sprites
	banner_life = image_load(BANNER_LIFE);
	banner_bomb = image_load(BANNER_BOMB);
	banner_range = image_load(BANNER_RANGE);
	banner_line = image_load(BANNER_LINE);
}

static void banner_unload() {
	// numbers imgs
	for (int i = 0; i < 10; i++) {
		SDL_FreeSurface(numbers[i]);
	}

	// other banner sprites
	SDL_FreeSurface(banner_life);
	SDL_FreeSurface(banner_bomb);
	SDL_FreeSurface(banner_range);
}
static void pause_menu_load()
{
	pause_menu_img[0] = image_load(PAUSE_TITLE);
	pause_menu_img[1] = image_load(PAUSE_RESUME);
	pause_menu_img[2] = image_load(PAUSE_SAVE);
	pause_menu_img[3] = image_load(PAUSE_QUIT);
}
static void pause_menu_unload()
{
	for (int i = 0; i < 4; i++)
		SDL_FreeSurface(pause_menu_img[i]);

}
static void map_load() {
	// Sprite loading
	tree = image_load(MAP_TREE);
	box = image_load(MAP_CASE);
	key = image_load(MAP_KEY);
	stone = image_load(MAP_STONE);
	door_opened = image_load(MAP_DOOR_OPENED);
	door_closed = image_load(MAP_DOOR_CLOSED);
	
}

static void map_unload() {
	SDL_FreeSurface(tree);
	SDL_FreeSurface(box);
	SDL_FreeSurface(key);
	SDL_FreeSurface(stone);
	SDL_FreeSurface(door_opened);
	SDL_FreeSurface(door_closed);
}

static void bomb_load(){
	bomb4 = image_load(BOMB_TTL4);
	bomb3 = image_load(BOMB_TTL3);
	bomb2 = image_load(BOMB_TTL2);
	bomb1 = image_load(BOMB_TTL1);
	explosion = image_load(EXPLOSION);
}

static void bomb_unload(){
	SDL_FreeSurface(bomb1);
	SDL_FreeSurface(bomb2);
	SDL_FreeSurface(bomb3);
	SDL_FreeSurface(bomb4);
	SDL_FreeSurface(explosion);
}

static void bonus_load() {
	bonus[0] = NULL;
	bonus[BONUS_BOMB_RANGE_INC] = image_load(IMG_BONUS_BOMB_RANGE_INC);
	bonus[BONUS_BOMB_RANGE_DEC] = image_load(IMG_BONUS_BOMB_RANGE_DEC);
	bonus[BONUS_BOMB_NB_INC] = image_load(IMG_BONUS_BOMB_NB_INC);
	bonus[BONUS_BOMB_NB_DEC] = image_load(IMG_BONUS_BOMB_NB_DEC);
}

static void bonus_unload() {
	for (int i = 0; i < NB_BONUS; i++)
		if(bonus[i])
			SDL_FreeSurface(bonus[i]);
}

static void player_load() {
	player_img[WEST] = image_load(PLAYER_LEFT);
	player_img[EAST] = image_load(PLAYER_RIGHT);
	player_img[NORTH] = image_load(PLAYER_UP);
	player_img[SOUTH] = image_load(PLAYER_DOWN);
}
static void monster_load() {
	monster_img[WEST] = image_load(MONSTER_LEFT);
	monster_img[EAST] = image_load(MONSTER_RIGHT);
	monster_img[NORTH] = image_load(MONSTER_UP);
	monster_img[SOUTH] = image_load(MONSTER_DOWN);
}

static void player_unload() {
	for (int i = 0; i < 4; i++)
		SDL_FreeSurface(player_img[i]);
}
static void monster_unload() {
	for (int i = 0; i < 4; i++)
		SDL_FreeSurface(monster_img[i]);
}

void sprite_load() {
	map_load();
	bonus_load();
	banner_load();
	pause_menu_load();
	player_load();
	bomb_load();
	monster_load();
}

void sprite_free() {
	map_unload();
	bonus_unload();
	banner_unload();
	pause_menu_unload();
	player_unload();
	bomb_unload();
	monster_unload();
}

SDL_Surface* sprite_get_number(short number) {
	assert(number >= 0 && number < 9);
	return numbers[number];
}

SDL_Surface* sprite_get_player(enum direction direction) {
	assert(player_img[direction]);
	return player_img[direction];
}
SDL_Surface* sprite_get_monster(enum direction direction) {
	assert(monster_img[direction]);
	return monster_img[direction];
}

SDL_Surface* sprite_get_banner_life() {
	assert(banner_life);
	return banner_life;
}

SDL_Surface* sprite_get_banner_bomb() {
	assert(banner_bomb);
	return banner_bomb;
}

SDL_Surface* sprite_get_banner_line() {
	assert(banner_line);
	return banner_line;
}

SDL_Surface* sprite_get_banner_range() {
	assert(banner_range);
	return banner_range;
}
SDL_Surface** sprite_get_pause_menu()
{
	assert(*pause_menu_img); return pause_menu_img;
}
SDL_Surface* sprite_get_bonus(enum bonus_type bonus_type) {
	assert(bonus[bonus_type]);
	return bonus[bonus_type];
}

SDL_Surface* sprite_get_tree() {
	assert(tree);
	return tree;
}

SDL_Surface* sprite_get_box() {
	assert(box);
	return box;
}

SDL_Surface* sprite_get_key() {
	assert(key);
	return key;
}

SDL_Surface* sprite_get_stone() {
	assert(stone);
	return stone;
}

SDL_Surface* sprite_get_door_opened() {
	assert(door_opened);
	return door_opened;
}

SDL_Surface* sprite_get_door_closed() {
	assert(door_closed);
	return door_closed;
}

SDL_Surface* sprite_get_bomb4(){
	assert(bomb4);
	return bomb4;
}

SDL_Surface* sprite_get_bomb3(){
	assert(bomb3);
	return bomb3;
}

SDL_Surface* sprite_get_bomb2(){
	assert(bomb2);
	return bomb2;
}


SDL_Surface* sprite_get_bomb1(){
	assert(bomb1);
	return bomb1;
}

SDL_Surface* sprite_get_explosion(){
	assert(explosion);
	return explosion;
}