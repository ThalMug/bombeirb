#include "monster.h"
#include "map.h"
#include "Vector2.h"
#include "rng.h"
#include <stdlib.h>
#include <assert.h>
#include "player.h"
#include "game.h"

const static Vector2 directions[4] = {
	{0, -1},
	{1,  0},
	{0,  1},
	{-1, 0}
};

typedef struct monster
{
    int x, y;
    enum direction direction;
    struct map* map;
    game* game;
    SDL_TimerID timer;
} monster;
// create new monster
monster* monster_new(map* map, game* g, int x, int y, direction direction)
{
    monster* m = malloc(sizeof(monster));
    m->x = x; m->y = y;
    m->game = g;
    m->direction = direction;
    m->map = map;
    m->timer = SDL_AddTimer(MONSTER_MOVE_DELAY, timerCallback, m);
    return m;
}
int monster_move(map* map, monster* m, enum direction d)
{
    Vector2 v = directions[d];
    if(monster_check_move(map, m, v))
    {
        m->direction = d;
        map_cell_move(map, m->x, m->y, v);
        m->x += v.x;
        m->y += v.y;
        map_set_cell_type(map, m->x, m->y, 0x60 | d);
        player* p = map_get_player(map);
        if(player_should_get_hit(p, map))
        {
            player_hit(p, 1);
            player_init_damage_timer(p, map, MONSTER_DAMAGE_DELAY);
        }
        return 1;
    }
    return 0;
}
int monster_check_move(map* map, monster* m, Vector2 d)
{
    assert(map); assert(m);
    if(!map_is_inside(map, m->x+d.x, m->y+d.y))
        return 0;
    if(map_get_cell_type(map, m->x+d.x, m->y+d.y))
        return 0;
    return 1;
}
// the function to call every second to move the monster at random
Uint32 timerCallback(Uint32 delay, void* param)
{
    monster* m = (monster*)param;
    // if we're on pause, just wait
    while (game_is_on_pause(m->game))
    {
        SDL_Delay(TIMER_RESOLUTION);
    }
    // generate a new random direction until the move is valid
    direction d;
    do
    {
        d = rng_rand() & 3;
    } while (!monster_check_move(m->map, m, directions[d]));
    monster_move(m->map, m, d);
    // return the delay before the next move
    return MONSTER_MOVE_DELAY;
}
void monster_free(monster* m)
{
    SDL_RemoveTimer(m->timer);
    free(m);
}