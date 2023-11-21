#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <player.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>
#include <bomb.h>
#include <map.h>
#include "game.h"

struct bomb{ 
    int x, y;
    int timer;
    int bomb_number;
    enum bomb_type bomb_type;
    struct map* map;
    game* game;
    SDL_TimerID explosion_timer;
};

struct bomb* bomb_tab[9] = {NULL}; //struct pointer which will contain the bomb information

struct bomb* bomb_init(int x, int y){ //Bomb creation and initialization
    struct bomb* bomb = malloc(sizeof(*bomb));
    if(!bomb)
        error("Memory Error");
    bomb->bomb_type = 0;
    bomb->x = x;
    bomb->game = NULL;
    bomb->y = y;
    bomb->timer = SDL_GetTicks();
    return bomb;
};

void bomb_tab_add(int x, int y, struct map* map, game* g) //Will add a bomb to the bomb_tab 
{
    assert(map && g);
    int i=0;
    while(bomb_tab[i] && i < 8){
        i++;
    }
    bomb_tab[i] = bomb_init(x,y);
    (bomb_tab[i])->map = map;
    (bomb_tab[i])->game = g;
}

struct bomb* bomb_seek(int x, int y){ //with a given x and y, it will find the bomb that correspond  
    for(int i=0;i<8;i++){
        if(bomb_tab[i] != NULL){
            if((bomb_get_x(bomb_tab[i])*40 == x) && (bomb_get_y(bomb_tab[i])*40 == y)){
                return bomb_tab[i];
            }
        }
    }
    return NULL;
}

void bomb_erase(int x, int y){ //Will erease the bomb with a given x and y 
    for(int i=0;i<9;i++){
        if(bomb_tab[i] != NULL){    
            if((bomb_get_x(bomb_tab[i])*40 == x) && (bomb_get_y(bomb_tab[i])*40 == y)){
                free(bomb_tab[i]);
                bomb_tab[i] = NULL;
            }
        }
    }
}

int bomb_get_x(struct bomb* bomb){ //get the x of a bomb
    assert(bomb);
    return bomb->x;
}

int bomb_get_y(struct bomb* bomb){ //get the y of a bomb
    assert(bomb);
    return bomb->y;
}

int bomb_get_timer(struct bomb* bomb){ //Will get the bomb timer 
    assert(bomb);
    return (SDL_GetTicks() - bomb->timer);
}
void bomb_set_timer(struct bomb* bomb, int t) //set the bomb timer
{
    assert(bomb); bomb->timer = SDL_GetTicks() + t;
}
bomb_data bomb_get_data(bomb* bomb)
{
    assert(bomb);
    return (bomb_data){ bomb->x, bomb->y, bomb->timer };
}

int bomb_get_count()
{
    int i =0;
    for(; bomb_tab[i]; i++);
    return i;   
}
int change_bomb(struct bomb* bomb){
    if(bomb->bomb_type == 4)
        return 0;
    bomb->bomb_type += 1;
    return 1;
}

int bomb_get_type(struct bomb* bomb){
    assert(bomb);
    return bomb->bomb_type;
}
void bomb_set_type(struct bomb* bomb, bomb_t type)
{
    assert(bomb); bomb->bomb_type = type;
}
int bomb_timer(struct bomb* bomb){
    return (bomb_get_timer(bomb) / 1000);
}

int explosion_assert(int i, int j,struct map* map){
    if(map_is_inside(map,i,j)){
        if(map_get_cell_type(map,i,j) == CELL_BOX){
            return 1;
        }
        else if((map_get_cell_type(map,i,j) & 0xf0) == CELL_BONUS){
            return 0;
        }
        else if((map_get_cell_type(map,i,j) != (cell_t)(CELL_STONE))){
            return 1;
        }
    }
    
    return 0;

}
struct bomb** get_bombs(){ return bomb_tab; }

void box_explosion(int i, int j, struct map* map){
    cell_t type = map_get_cell_type(map,i,j);
    map_set_cell_type(map,i,j,((type & 0x0f) | 0x50));    
}

void explosion_display(int x, int y,struct map* map){ //will be called after the bomb timer is > than 4
    int range = player_get_range(map_get_player(map));
    for(int i=x/40-range+1;i<x/40;i++){

        if(map_is_inside(map,i,y/40) && explosion_assert(i,y/40,map)){
            if((map_get_cell_type(map,i,y/40) & 0xf0) == (cell_t)CELL_BOX){
                box_explosion(i,y/40,map);
            }
            else
                map_set_cell_type(map,i,y/40,CELL_EXPLOSION);
        }
    }
    for(int i=x/40;i<x/40+range;i++){

        if(map_is_inside(map,i,y/40) && explosion_assert(i,y/40,map)){
            if((map_get_cell_type(map,i,y/40) & 0xf0) == (cell_t)CELL_BOX){
                box_explosion(i,y/40,map);
            }
            else
                map_set_cell_type(map,i,y/40,CELL_EXPLOSION);
        }
    }
    for(int i=y/40-range+1;i<y/40;i++){
        if(map_is_inside(map,x/40,i) && explosion_assert(x/40,i,map)){
            if((map_get_cell_type(map,i,y/40) & 0xf0) == (cell_t)CELL_BOX){
                box_explosion(x/40,i,map);
            }
            else
                map_set_cell_type(map,x/40,i,CELL_EXPLOSION);  
        }       
    }
    for(int i=y/40;i<y/40+range;i++){
        if(map_is_inside(map,x/40,i) && explosion_assert(x/40,i,map)){
            if((map_get_cell_type(map,i,y/40) & 0xf0) == (cell_t)CELL_BOX){
                box_explosion(x/40,i,map);
            }
            else
                map_set_cell_type(map,x/40,i,CELL_EXPLOSION);  
        }       
    }
}
void bomb_set_map(struct bomb* b, map* m)
{
    assert(b && m); b->map = m;
}
void bomb_set_game(struct bomb* b, game* g)
{
    assert(b && g); b->game = g;
}
Uint32 explosion_swipe(Uint32 delay, void * p){ //delete the explosion and replace with cell_empty after 1 second of display
    struct bomb * b = (bomb*)p;
    struct map * map = b->map;
    int range = player_get_range(map_get_player(map));
    int x = bomb_get_x(p), y = bomb_get_y(p);

    for(int i=x-range+1;i<range+x;i++){ 
        if(map_is_inside(map,i,y) && explosion_assert(i,y,map)){
            map_set_cell_type(map,i,y,CELL_EMPTY);
        }
    }
    for(int i=y-range+1;i<range+y;i++){
        if(map_is_inside(map,x,i) && explosion_assert(x,i,map)){
            map_set_cell_type(map,x,i,CELL_EMPTY);
        }       
    }
    bomb_erase(x,y);
    return 0;
}

void bomb_set_explosion_timer(struct bomb* bomb,Uint32 delay){
    bomb->explosion_timer = SDL_AddTimer(delay,explosion_swipe,bomb);
}

void bomb_display(struct bomb* bomb,int x, int y,struct map* map,struct player * player){ //Display the bomb to the screen
    switch(bomb_timer(bomb)){
        case 0:
            window_display_image(sprite_get_bomb4(),x,y);
            break;
        case 1:
            window_display_image(sprite_get_bomb3(),x,y);
            break;
        case 2:
            window_display_image(sprite_get_bomb2(),x,y);
            break;
        case 3:
            window_display_image(sprite_get_bomb1(),x,y);
            break;
    }
}