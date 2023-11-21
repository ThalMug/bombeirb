#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE (64*64)

int readGrid(FILE* f, size_t count, unsigned char* grid)
{
    size_t n = 0;
    for (size_t i = 0; i < count; i++)
        n += fscanf(f, " %02x", (int*)(grid+i));
    return n;
}

int main(int argc, char const *argv[])
{
    FILE* in = fopen(argv[1], "r");
    FILE* out = fopen("../maps/maps.m", "wb");
    if(!in) return EXIT_FAILURE;
    if(!out)
    {
        printf("Error: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    unsigned char grid[MAX_SIZE] = {0};
    unsigned int width, height;
    fscanf(in, "%u%u", &width, &height);
    size_t n = readGrid(in, width*height, grid);
    while(n == (width*height))
    {
        fwrite(&width, 1, 1, out);
        fwrite(&height, 1, 1, out);
        fwrite(grid, 1, width*height, out);
        fscanf(in, "%d%d", (int*)&width, (int*)&height);
        n = readGrid(in, width*height, grid);
    }
    return 0;
}
