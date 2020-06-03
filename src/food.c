#include "food.h"

// create's a random food on the board
void create_new_food(point* food, snake_body* snake){
    food->x = 40 + (rand() % 39) * 15;
    food->y = 40 + (rand() % 39) * 15;
}

// draws a food point
void draw_food(point food){
    al_draw_filled_rectangle(
        food.x,
        food.y,
        food.x + 15,
        food.y + 15,
        al_map_rgb(255,0,0)
    );
}
