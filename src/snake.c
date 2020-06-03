#include "snake.h"

// initializes snake with length 7
void init_snake(snake_body* snake){
    // for each block in snake body
    for (int i = 0; i < 1600; i++){
        // for initial length of 7
        if (i < 7){
            snake->active = 1;  // make block active
        } else {
            // for all other blocks
            snake->active = 0;  // make them inactive
        }
        snake->position.x = 190 - (15 * i); // initialize x coordinate
        snake->position.y = 190;    // initialize y coordinate
        snake++;    // next body block
    }
}

// function moves snake in given direction. returns 1 if food is consumed during movement otherwise zero
// takes in a pointer to the snake it needs to move, direction in which to move and location of food
int move_snake(snake_body* snake, int direction, point *food){
    // moves snake by storing locations. temp1 is first set to point the snake needs to
    // snakes head is then stored in temp2
    // snakes head is updated to value in temp1
    // temp1's value is updated to value from temp 2
    // next body element stores its position in temp2
    // its position is updated to value in temp1
    // temp1 is updated to temp2's value
    // this happens for every cell that is active in snake's array.
    // if food had been consumed while moving snake's head, grow flag is set
    // it sets the first inactive cell in snake to active and updates it's position to
    // value stored in temp2 which was last position of snake's tail
    // temp3 stores the last value of snake's tail where new element is to be added whenever snake eat's food

    point temp1;        // temporary location 1
    point temp2;        // temporary location 2
    point temp3;
    int new_food = 0;   // sets to one if new food is required
    temp1 = snake->position;    // initialize temp1 with location of snake's head
    int grow = 0;       // initialize this flag as off


    // updates temp1 to new location according to the direction where it needs to move
    if (direction == RIGHT){
        temp1.x += 15;
    } else if (direction == LEFT){
        temp1.x -= 15; 
    } else if (direction == DOWN){
        temp1.y += 15;
    } else if (direction == UP){
        temp1.y -= 15;
    }

    // out of bound checks
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

    // if new position of head is same as position of food, set grow flag to indicate that the food is consumed and that
    // snake needs to grow by length of one
    if ((food->x == temp1.x) && (food->y == temp1.y)){
        grow = 1;
        new_food = 1;
    }

    // for every snake_body element,
    for (int i = 0; i < 1600; i++){
        // update values of body elements
        temp3 = temp2;
        temp2 = (snake + i)->position;
        (snake + i)->position = temp1;
        temp1 = temp2;

        // if this block is inactive
        if ((snake + i)->active == 0){
            // if snake needs to grow
            if(grow == 1){
                // set this inactive block to active
                ((snake + i)->active) = 1;
                // set this elements position as the tails last location
                (snake + i)->position = temp3;
            }
            // break out if this block was inactive or length was extended
            break;
        }
    }
    return new_food;    // return 1 if new food is required, 0 otherwise
}

// detects if snake's head has collided with body
// checks each active block of snake and sees if it has same position as head. if it is, it returns 1 meaning snake
// has collided, otherwise returns 0
int detect_snake_collisson(snake_body* snake){
    point head = snake->position;   // save position of head
    int flag = 0;   // flag sets to one if collision occurs
    // checks every block for collision
    for (int i = 1; i < 1600; i++){
        // checks for collision
        if ((snake + i)->position.x == head.x && (snake + i)->position.y == head.y && (snake + i)->active == 1){
            flag = 1;
            break;
        }
        // checks for active block
        if ((snake + i)->active == 0){
            break;
        }
    }
    return flag;
}

// draws snake's body on display
void draw_snake(snake_body* snake){
    // draws head
    al_draw_filled_rectangle(
            (float)((snake)->position.x),
            (float)((snake)->position.y),
            (float)((snake)->position.x + 15),
            (float)((snake)->position.y + 15),
            al_map_rgb(255, 0, 255)
            );
    // draws eye
    al_draw_filled_circle(
            (float)((snake)->position.x) + 7.5,
            (float)((snake)->position.y) + 7.5,
            (float)5.0,
            al_map_rgb(0, 255, 0)
            );
    // draws every other part of snake's body. breaks whenever it encounters an inactive block
    for(int i = 1; i < 1600; i++){
        // breaks if current block is invalid
        if ((snake + i)->active == 0){
            break;
        }
        al_draw_filled_rectangle(
                (float )((snake + i)->position.x),
                (float)((snake + i)->position.y),
                (float)((snake + i)->position.x + 15),
                (float)((snake + i)->position.y + 15),
            al_map_rgb(255, 0, 255)
        );
    }
}
