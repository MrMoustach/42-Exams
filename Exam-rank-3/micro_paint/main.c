#include <stdio.h>
#include <stdlib.h>


typedef struct  s_rectangle
{
    char        type;
    float       x;
    float       y;
    float       width;
    float       height;
    char        c;
    struct s_rectangle *next;
}               t_rectangle;

typedef struct  s_zone
{
    char    zone[301][301];
    float   width;
    float   height;
    char    bg;
    int     error;
}               t_zone;

t_zone *zone;
t_rectangle *rect;

void get_zone(FILE *file)
{
    int ret;
    int x;
    int y;

    zone = malloc(sizeof(t_zone));
    ret = fscanf(file, "%f %f %c\n", &zone->width,&zone->height, &zone->bg);
    if (ret < 3)
    {
        zone->error = -1;
        return ;
    }
    x = 0;
    y = 0;
    while (y < 301)
    {
        x = 0;
        while (x < 301)
        {
            zone->zone[y][x] = 0;
            if (y < zone->height && x < zone->width)
                zone->zone[y][x] = zone->bg; 
            x++;
        }
        y++;
    }
    zone->error = 0;
}

void    get_rect(FILE *file)
{
    int ret;

    while ((ret = fscanf(file, "%c %f %f %f %f %c", &rect->type, &rect->x, &rect->y, &rect->width, &rect->height, &rect->c)) == 6)
    {
        
    }
}

int main(int ac, char *av[])
{
    FILE *file;
    int x, y;

    file = fopen(av[1], "r");
    get_zone(file);
    y = 0;
    x = 0;
    while (y < zone->width)
    {
        x = 0;
        while (x < zone->height)
        {
            printf("%c", zone->zone[y][x]);
            x++;
        }
        printf("\n");
        y++;
    }
    return (0);
}