#ifndef CORE_H
#define CORE_H

#include "base.h"
#include "snake.h"
#include "food.h"

void error(char* msg);

int start_game_screen(
        ALLEGRO_EVENT_QUEUE* queue,
        ALLEGRO_TIMER* timer,
        ALLEGRO_DISPLAY* display,
        ALLEGRO_FONT* font,
        int* endgame_flag
        );

unsigned int play_snake(
    ALLEGRO_EVENT_QUEUE* queue,
    ALLEGRO_TIMER* timer,
    ALLEGRO_DISPLAY* display,
    ALLEGRO_FONT* font,
    int display_grid,
    int* endgame_flag
    );

void game_over_screen(
        ALLEGRO_EVENT_QUEUE* queue,
        ALLEGRO_TIMER* timer,
        ALLEGRO_DISPLAY* display,
        ALLEGRO_FONT* font,
        unsigned  int score
        );

void play_game();


#endif