#ifndef BUTTON_H
#define BUTTON_H

#include "constant.h"

typedef struct button button;

button* button_new(SDL_Surface* sprite, int x, int y, int w, int h, int (*action)(void*));
void button_free(button* b);

int button_has_been_clicked(button* b, int x, int y);
int button_call_action(button* b, void* param);

void button_display(button *b);

#endif