#include "core.h"
#include "snake.h"
#include "food.h"

void error(char* msg) {
    fprintf(stderr, "%s : %s\n", msg, strerror(errno));
    exit(1);
}

unsigned int play_snake(
    ALLEGRO_EVENT_QUEUE* queue,
    ALLEGRO_TIMER* timer,
    ALLEGRO_DISPLAY* display,
    ALLEGRO_FONT* font,
    int display_grid
){
    ALLEGRO_EVENT event;
    unsigned int score = 0;
    point food;
    snake_body snake[1600];
    int direction = RIGHT;
    int new_food = 0;
    int keypress = 0;
    int done = 0;
    int redraw = 1;
    init_snake(snake);
    create_new_food(&food, snake);
    while(1) {

        al_wait_for_event(queue, &event);

        switch(event.type){
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = 1;
            break;
        case ALLEGRO_EVENT_TIMER:
            new_food = move_snake(snake, direction, food);
            redraw = 1;
            break;
        case ALLEGRO_EVENT_KEY_DOWN:
            if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                done = 1;
                break;
            }
            if(event.keyboard.keycode == ALLEGRO_KEY_UP){
                keypress = UP;
            } else if(event.keyboard.keycode == ALLEGRO_KEY_DOWN){
                keypress = DOWN;
            } else if(event.keyboard.keycode == ALLEGRO_KEY_LEFT){
                keypress = LEFT;
            } else if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT){
                keypress = RIGHT;
            }
            if(direction != -keypress && direction != keypress){
                new_food = move_snake(snake, keypress, food);
                direction = keypress;
                break;
            }
        }

        if (new_food){
            create_new_food(&food, snake);
            score++;
        }

        if (done){
            break;
        }

        // redraw the display
        if (redraw && al_is_event_queue_empty(queue)){
            al_clear_to_color(al_map_rgb(0, 0, 0));
            
            // display score
            al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 10, 0, "Score: %i", score);
            
            // draw playing area
            al_draw_rectangle(40,40, 640, 640, al_map_rgb(255, 255, 0), 2);
            
            if (display_grid){
                for(int i = 55; i < 640; i += 15){
                    al_draw_line(40, i, 640, i, al_map_rgb(255, 255, 0), 1);
                    al_draw_line(i, 40, i, 640, al_map_rgb(255, 255, 0), 1);  
                }
            }

            al_draw_text(font, al_map_rgb(255, 255, 255), 290, 660, 0, "Press Esc to exit...");

            // draw food
            draw_food(food);

            // draw snake
            draw_snake(snake);

            // send to display to draw
            al_flip_display();
            redraw = 0;
        }
    }
    return score;
}

void play_game(){
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
    ALLEGRO_TIMER* timer = al_create_timer(1.0/30.0);
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
    al_register_event_source(queue, al_get_timer_event_source(timer));

    unsigned int score = 0;
    al_start_timer(timer);
    score = play_snake(
        queue,
        timer,
        disp,
        font, 
        0
    );

    printf("Score: %i\n", score);

    // Destructors
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    return;
}
