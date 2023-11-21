#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <SDL/SDL.h>
#include "constant.h"

typedef struct button tool_button;

typedef enum tool {
    TOOL_BOX,
    TOOL_PLAYER_SPAWN,
    TOOL_SCENERY,
    TOOL_MONSTER,
    TOOL_BONUS,
    TOOL_KEY,
    TOOL_ENTRANCE_DOOR,
    TOOL_EXIT_DOOR,
} tool_t;

#define TOOL_COUNT 9

typedef struct toolbar toolbar;

void toolbar_display(toolbar*);
toolbar* toolbar_new();
void toolbar_free(toolbar*);
tool_button* toolbar_get_button(toolbar* tb, tool_t index);
tool_t toolbar_get_current_tool(toolbar* tb);
void toolbar_set_current_tool(toolbar* tb, tool_t tool);

#endif