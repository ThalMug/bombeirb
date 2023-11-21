#ifndef CONTEXT_BAR_H
#define CONTEXT_BAR_H

#include "map.h"

typedef enum tool tool_t;
typedef enum context_tool{
    TOOL_TREE,
    TOOL_STONE,

    TOOL_BOX_RANGE_INC,
    TOOL_BOX_RANGE_DEC,
    TOOL_BOX_BOMB_INC,
    TOOL_BOX_BOMB_DEC,
    TOOL_BOX_LIFE,
    TOOL_BOX_MONSTER,

    TOOL_BONUS_RANGE_INC,
    TOOL_BONUS_RANGE_DEC,
    TOOL_BONUS_BOMB_INC,
    TOOL_BONUS_BOMB_DEC,
    TOOL_BONUS_LIFE,

    TOOL_OPEN_DOOR,
    TOOL_CLOSED_DOOR,
} cxt_tool_t;


// typedef struct button tool_button;
typedef struct context_bar context_bar;

context_bar* context_bar_new(tool_t toolType);
void context_bar_free(context_bar*);

#endif