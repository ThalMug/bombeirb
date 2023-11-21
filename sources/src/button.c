#include "button.h"
#include "misc.h"
#include "constant.h"
#include "window.h"

typedef struct button
{
    SDL_Surface* sprite;
    int x, y;
    int w, h;
    int (*action)(void*);
}button;


button* button_new(SDL_Surface* sprite, int x, int y, int w, int h, int (*action)(void*))
{
    button* b = malloc(sizeof(button));
    b->action = action;
    b->x = x; b->y=y;
    b->w=w; b->h=h;
    b->sprite = sprite;

    return b;
}
void button_free(button* b)
{
    assert(b); free(b);
}
void button_display(button *b)
{
    assert(b && b->sprite);
    window_display_image(b->sprite, b->x, b->y);
}
int button_has_been_clicked(button* b, int x, int y)
{
    assert(b);
    return (IS_BETWEEN(x, b->x, b->x+b->w) && IS_BETWEEN(y, b->y, b->y+b->h));
}
int button_call_action(button* b, void* param)
{
    assert(b);
    return b->action? b->action(param): -1;
}