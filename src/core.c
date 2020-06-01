#include "core.h"
#include "snake.h"
#include "food.h"

// Exits whit exit code 1 in case an error occurs. Displays error message using errno.h
void error(char* msg) {
    // output to STDERR the message to be displayed along with error code description
    fprintf(stderr, "%s : %s\n", msg, strerror(errno));
    exit(1);
}

// The main gameplay happens here. It takes in a queue, a timer, a display, a font and an option to display the grid.
// returns the score obtained during that gameplay
unsigned int play_snake(
    ALLEGRO_EVENT_QUEUE* queue,
    ALLEGRO_TIMER* timer,
    ALLEGRO_DISPLAY* display,
    ALLEGRO_FONT* font,
    int display_grid
){
    ALLEGRO_EVENT event;    // registers a variable that store events from event queue
    
    // Game Variables
    unsigned int score = 0; // Stores the score of a player
    point food;             // registers a point on the board as food
    snake_body snake[1600]; // creates a 1600 element(total no of cells on board) array of snake_body structs to denote a snake
    int direction = RIGHT;  // start direction of snake
    int new_food = 0;       // this variable is a flag that is set when there is a need to create a new food point
    int keypress = RIGHT;   // this is used to store the last direction snake was instructed to move by the user
    int done = 0;           // a flag which is set when the game is over, or the user asked to end the game, or user closes the window
    int redraw = 1;         // a flag which is set when the display needs to be redrawn. Set using timer to have a constant frame rate

    
    init_snake(snake);              // initializes the snake
    create_new_food(&food, snake);  // creates the first food element

    // Game Loop
    while(1) {
        // waits until an event is registered
        // includes frame rate timer, keypresses and window closings
        al_wait_for_event(queue, &event);

        // decodes the event type and proceeds accordingly
        switch(event.type){
        // whenever the display is closed by clicking 'x'
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = 1;   
            break;

        // whenever framerate timer goes off
        case ALLEGRO_EVENT_TIMER:
            new_food = move_snake(snake, direction, food); // moves snake in last direction
            redraw = 1;
            break;

        // whenever a key is pressed
        case ALLEGRO_EVENT_KEY_DOWN:
            // if escape key is pressed
            if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                // set flag to exit the game
                done = 1;
                break;
            }

            // associate all other keypresses with their respective directions
            if(event.keyboard.keycode == ALLEGRO_KEY_UP){
                keypress = UP;
            } else if(event.keyboard.keycode == ALLEGRO_KEY_DOWN){
                keypress = DOWN;
            } else if(event.keyboard.keycode == ALLEGRO_KEY_LEFT){
                keypress = LEFT;
            } else if(event.keyboard.keycode == ALLEGRO_KEY_RIGHT){
                keypress = RIGHT;
            }

            // change direction if new direction is not same as or opposite of old direction 
            if(direction != -keypress && direction != keypress){
                new_food = move_snake(snake, keypress, food);   // move snake in changed direction
                direction = keypress;   // update current direction
                break;
            }
        }

        // create new food if previous was consumed
        if (new_food){
            create_new_food(&food, snake);
            score++; // increment score
        }

        // break out of game loop if done with playing
        if (done){
            break;
        }

        // redraw the display if redraw flag is set and event queue is empty
        if (redraw && al_is_event_queue_empty(queue)){
            al_clear_to_color(al_map_rgb(0, 0, 0));     // background color
            al_draw_text(font, al_map_rgb(255, 255, 255), 290, 660, 0, "Press ESC to exit..."); // display exit key
            al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 10, 0, "Score: %i", score);      // display score
            al_draw_rectangle(40,40, 640, 640, al_map_rgb(255, 255, 0), 2);     // game boundary
            
            // if display grid flag is set
            if (display_grid){
                // for each 15px interval between 40 and 640
                for(int i = 55; i < 640; i += 15){
                    al_draw_line(40, i, 640, i, al_map_rgb(255, 255, 0), 1);    // draw horizontal line
                    al_draw_line(i, 40, i, 640, al_map_rgb(255, 255, 0), 1);    // draw vertical line
                }
            }
            draw_food(food);    // display food
            draw_snake(snake);  // display snake

            al_flip_display();  // draw the display
            redraw = 0;     // unset redraw flag so that display is drawn only when next timer event occurs
        }
    }
    return score;   // return the score obtained
}

// this function is used to play the game. it initializes Allegro and its addons. It also initializes 
// other things required such as event queue, timer and display
void play_game(){
    // initialize allegro
    if (!al_init()){
        error("Cannot Initialize Allegro Core");
    }
    // initialize allegro primitives addon
    if (!al_init_primitives_addon()){
        error("Cannot Initialize Allegro Primitives");
    }
    // initialize allegro font addon
    if (!al_init_font_addon()){
        error("Cannot Initialize Allegro Fonts");
    }
    // enable keyboard
    if (!al_install_keyboard()){
        error("Cannot Install Keyboard");
    }

    // constructs a Timer that times out every 1/30th second
    // Used to create 30FPS display
    // returns NULL pointer if unable to create
    ALLEGRO_TIMER* timer = al_create_timer(1.0/30.0);
    if (!(uintptr_t)timer) {
        error("Cannot create Timer");
    }

    // constructs a a queue where all alerts are sent to be handled
    // returns NULL pointer if unable to create
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    if (!(uintptr_t)queue) {
        error("Cannot create Event Queue");
    }

    // constructs a 640x480 Window
    // returns NULL pointer if unable to create
    ALLEGRO_DISPLAY* disp = al_create_display(680, 680);
    if (!(uintptr_t)disp) {
        error("Cannot create Display Window");
    }

    // contructs font to be used
    // returns NULL pointer if unable to create
    ALLEGRO_FONT* font = al_create_builtin_font();
    if (!(uintptr_t)font) {
        error("Cannot create font");
    }

    // Regsiter events that would go into Event Queue
    al_register_event_source(queue, al_get_keyboard_event_source());    // Keyboard events
    al_register_event_source(queue, al_get_display_event_source(disp)); // Display events
    al_register_event_source(queue, al_get_timer_event_source(timer));  // Timer events

    unsigned int score = 0;     // initialize score to 0
    al_start_timer(timer);      // start timer to maintain constant frame rate accross gameplays
    
    // TODO: create start menu screen

    // call main gameplay function with display grid flag and save score
    score = play_snake(queue, timer, disp, font, 0);

    printf("Score: %i\n", score);       // TODO: replace with game over screen

    // Destructors
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    return;
}
