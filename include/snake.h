#ifndef SNAKE_H
#define SNAKE_H

#include "base.h"

typedef struct snake_body {
    int active;
    point position;
} snake_body;

void init_snake(snake_body* snake);
int move_snake(snake_body* snake, int direction, point food);
int detect_snake_collisson(snake_body* snake);
void draw_snake(snake_body* snake);

#endif