#include "snake.h"

void init_snake(snake_body* snake){
    for (int i = 0; i < 7; i++){
        snake->active=1; // set corresponding body block to active
        snake->position.x = 190 - (15 * i); // init x coordinate
        snake->position.y = 190; // init y coordinate
        snake++; // next body block
    }
    return;
}

int move_snake(snake_body* snake, int direction, point food){
    point temp1;
    point temp2;
    int new_food = 0;
    temp1 = snake->position;    
    if (direction == RIGHT){
        temp1.x += 15;
    } else if (direction == LEFT){
        temp1.x -= 15; 
    } else if (direction == DOWN){
        temp1.y += 15;
    } else if (direction == UP){
        temp1.y -= 15;
    }
    if (temp1.x < 40){
        temp1.x = 625;
    }
    if (temp1.y < 40){
        temp1.y = 625;
    }
    if (temp1.x > 625){
        temp1.x = 40;
    }
    if (temp1.y > 625){
        temp1.y = 40;
    }

    int grow = 0;
    if (food.x == temp1.x && food.y == temp1.y){
        grow = 1;
    }
    for (int i = 0; i < 1600; i++){
        if ((snake+i)->active == 0){
            if(grow){
                (snake + i)->active = 1;
                (snake + i)->position = temp2;
                new_food = 1;
            }
            break;
        }
        temp2 = (snake + i)->position;
        (snake + i)->position = temp1;
        temp1 = temp2;
    }
    return new_food;
}

int detect_snake_collisson(snake_body* snake){
    point head = snake->position;
    int flag = 0;
    for (int i = 1; i < 1600; i++){
        if ((snake + i)->position.x == head.x && (snake + i)->position.y == head.y){
            flag = 1;
            break;
        } 
        if ((snake + i)->active == 0){
            break;
        }
    }
    if (flag){
        return 1;
    } else {
        return 0;
    }
}

void draw_snake(snake_body* snake){
    for(int i = 0; i < 1600; i++){
        if ((snake + i)->active == 0){
            break;
        }
        al_draw_filled_rectangle(
            (snake + i)->position.x,
            (snake + i)->position.y,
            (snake + i)->position.x + 15,
            (snake + i)->position.y + 15,  
            al_map_rgb(255, 0, 255)
        );
    }
    return;
}
