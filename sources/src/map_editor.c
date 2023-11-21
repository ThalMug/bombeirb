
// #define BOMBEIRB_EDITOR
#ifdef BOMBEIRB_EDITOR

#include "SDL/SDL.h"
#include <stdio.h>
#include "map.h"
#include "constant.h"
#include "window.h"

int processEvents()
{
    SDL_Event evt;
    if(!SDL_PollEvent(&evt))
        return 1;
    switch (evt.type)
    {
    case SDL_QUIT:
        return 0;
        break;
    default:
        return 1;
        break;
    }
}

int main(int argc, char *argv[])
{
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
        printf("Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    window_create(800, 600);

    while(processEvents())
    {
        window_refresh();
    }
    SDL_Quit();
    return 0;
}


#endif