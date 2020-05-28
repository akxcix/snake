#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

// Displays an error messege and exits
void error(char* msg) {
    fprintf(stderr, "%s : %s\n", msg, strerror(errno));
    exit(1);
}

// denotes a point on our display
typedef struct point {
    int x;
    int y;    
} point;

typedef struct snake_body {
    int active;
    point position;
} snake_body;

void init_snake(snake_body* snake){
    for (int i = 0; i < 7; i++){
        snake->active=1; // set corresponding body block to active
        snake->position.x = 190 - (15 * i); // init x coordinate
        snake->position.y = 190; // init y coordinate
        snake++; // next body block
    }
    return;
}

// move the snake in given direction, also extends the snake length by 1 if
// head coincides with food. returns 1 if food is consumed
int move_snake(snake_body* snake, int direction, point food){
    point temp1;
    point temp2;
    int new_food = 0;
    temp1 = snake->position;    
    if (direction == 0){
        temp1.x += 15;
    } else if (direction == 1){
        temp1.y += 15;
    } else if (direction == 2){
        temp1.x -= 15;
    } else if (direction == 3){
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

// returns 1 if head of the snake coincides with any other block of body
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

void create_new_food(point* food){
    food->x = 40 + (rand() % 39) * 15;
    food->y = 40 + (rand() % 39) * 15;
}

void draw_food(point food){
    al_draw_filled_rectangle(
        food.x,
        food.y,
        food.x + 15,
        food.y + 15,
        al_map_rgb(255,0,0)
    );
    return;
}

int main() {
    // Initialize the library and its Addons
    if (!al_init()){
        error("Cannot Initialize Allegro Core");
    }
    // Initializes primitives
    if (!al_init_primitives_addon()){
        error("Cannot Initialize Allegro Primitives");
    }
    // Font
    if (!al_init_font_addon()){
        error("Cannot Initialize Allegro Fonts");
    }
    // Enable Keyboard Input
    if (!al_install_keyboard()){
        error("Cannot Install Keyboard");
    }

    // Constructs a Timer that times out every 1/30th second
    // Used to create 30FPS display
    ALLEGRO_TIMER* timer = al_create_timer(1.0/4.0);
    if (!(uintptr_t)timer) {
        error("Cannot create Timer");
    }

    // Constructs a a queue where all alerts are sent to be handled
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    if (!(uintptr_t)queue) {
        error("Cannot create Event Queue");
    }

    // Constructs a 640x480 Window. All 
    ALLEGRO_DISPLAY* disp = al_create_display(680, 680);
    if (!(uintptr_t)disp) {
        error("Cannot create Display Window");
    }

    ALLEGRO_FONT* font = al_create_builtin_font();
    if (!(uintptr_t)font) {
        error("Cannot create font");
    }

    // Regsiter events that would go into Event Queue
    al_register_event_source(queue, al_get_keyboard_event_source()); // Keyboard
    al_register_event_source(queue, al_get_display_event_source(disp)); // Display
    al_register_event_source(queue, al_get_timer_event_source(timer)); // Timer

    // stores 0 or 1 (flase and true respectively) for storing exit 
    // status. If 1, execution exits game loop
    int done = 0;

    // stores 0 or 1
    // during execution of the game loop, if this is set to 1, display is
    // redrawn and is reset to 0
    int redraw = 1;

    // Game Variables
    unsigned int score = 0;
    point food;
    snake_body snake[1600];
    int direction = 0;
    int new_food = 0;

    init_snake(snake);
    create_new_food(&food);

    ALLEGRO_EVENT event;
    // Game Loop
    al_start_timer(timer);
    while(1) {

        al_wait_for_event(queue, &event);

        switch(event.type){
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = 1;
            break;
        case ALLEGRO_EVENT_TIMER:
            if(direction == 3){
                new_food = move_snake(snake, 3, food);
            }else if(direction == 2){
                new_food = move_snake(snake, 2, food);
            }else if(direction == 1){
                new_food = move_snake(snake, 1, food);
            }else if(direction == 0){
                new_food = move_snake(snake, 0, food);
            }
            redraw = 1;
            break;
        case ALLEGRO_EVENT_KEY_DOWN:
            if(event.keyboard.keycode == ALLEGRO_KEY_UP){
                if(direction != 3){
                    new_food = move_snake(snake, 3, food);
                    direction = 3;
                    break;
                }
            }
            if(event.keyboard.keycode == ALLEGRO_KEY_DOWN){
                if(direction != 1){
                    new_food = move_snake(snake, 1, food);
                    direction = 1;
                    break;
                }
            }
            if(event.keyboard.keycode == ALLEGRO_KEY_LEFT){
                if(direction != 2){
                    new_food = move_snake(snake, 2, food);
                    direction = 2;
                    break;
                }
            }
            if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT){
                if(direction != 0){
                    new_food = move_snake(snake, 0, food);
                    direction = 0;
                    break;
                }
            }
            redraw = 1;
        }

        if (new_food){
            create_new_food(&food);
            score++;
        }

        if (done){
            break;
        }

        // redraw the display
        if (redraw && al_is_event_queue_empty(queue)){
            al_clear_to_color(al_map_rgb(0, 0, 0));
            
            // display score
            al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Score: %i", score);
            
            // draw playing area
            al_draw_rectangle(40,40, 640, 640, al_map_rgb(255, 255, 0), 1);
            for(int i = 55; i < 640; i += 15){
                al_draw_line(40, i, 640, i, al_map_rgb(255, 255, 0), 1);
                al_draw_line(i, 40, i, 640, al_map_rgb(255, 255, 0), 1);
                
            }

            // draw food
            draw_food(food);

            // draw snake
            draw_snake(snake);

            // send to display to draw
            al_flip_display();
            redraw = 0;
        }
    }

    // Destructors
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}