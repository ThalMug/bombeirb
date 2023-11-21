#ifndef BOMB_H_
#define BOMB_H_

#include <map.h>
#include <misc.h>


typedef enum bomb_type{
    BOMB4=0,
    BOMB3,
    BOMB2,
    BOMB1
}bomb_t;
typedef struct bomb_data
{
    unsigned char x, y;
    uint32_t timer;
}bomb_data;

typedef struct bomb bomb;

struct bomb* bomb_init(int x, int y);


void bomb_set_position(struct bomb* bomb, int x, int y);

int bomb_get_x(struct bomb* bomb);
int bomb_get_y(struct bomb* bomb);


int bomb_get_timer(struct bomb* bomb);
void bomb_set_timer(struct bomb* bomb, int t);

void bomb_set_map(struct bomb*, map*);
void bomb_set_game(struct bomb*, game*);

bomb_data bomb_get_data(bomb*);
void bomb_set_type(struct bomb* bomb, bomb_t type);

int change_bomb(struct bomb* bomb);

void display_bombs(int x, int y, enum bomb_type bomb_type);

void bomb_display(struct bomb* bomb,int x, int y,struct map* map,struct player *player);

void explosion_display(int x, int y,struct map* map);

int bomb_timer(struct bomb* bomb);

void bomb_tab_add(int x, int y, struct map* map, game* g);

struct bomb* bomb_seek(int x, int y);

void bomb_erase(int x, int y);

struct bomb** get_bombs();
int bomb_get_count();

void bomb_set_explosion_timer(struct bomb* bomb,Uint32 delay);

Uint32 explosion_swipe(Uint32 delay, void * p);

void box_explosion(int i, int j,struct map * map);

#endif /* BOMB_H_ */