#ifndef MONSTER_H
#define MONSTER_H

#include "constant.h"
#include "Vector2.h"
#include <SDL/SDL.h>

#define MONSTER_MOVE_DELAY 1000

typedef struct monster monster;
typedef struct map map;
typedef struct game game;

// monster creation/deletion
monster* monster_new(map* map, game* g, int x, int y, direction direction);
void monster_free(monster* m);

// movements
int monster_check_move(map* map, monster* m, Vector2 d);
int monster_move(map* map, monster* m, direction d);
Uint32 timerCallback(Uint32 delay, void* monster);

#endif