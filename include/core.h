#ifndef CORE_H
#define CORE_H

#include "base.h"
#include "snake.h"
#include "food.h"

void error(char* msg);

unsigned int play_snake(
    ALLEGRO_EVENT_QUEUE* queue,
    ALLEGRO_TIMER* timer,
    ALLEGRO_DISPLAY* display,
    ALLEGRO_FONT* font,
    int display_grid
);

void play_game();


#endif