#include "core.h"

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
    int display_grid,
    int* endgame_flag
){
    ALLEGRO_EVENT event;    // registers a variable that store events from event queue
    
    // Game Variables
    unsigned int score = 0; // Stores the score of a player
    point food;             // registers a point on the board as food
    snake_body snake[1600]; // creates a 1600 element(total no of cells on board) array of snake_body structs to denote a snake
    int direction = RIGHT;  // start direction of snake
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
            *endgame_flag = 1;
            done = 1;   
            break;

        // whenever framerate timer goes off
        case ALLEGRO_EVENT_TIMER:
            // moves snake in last direction
            if (move_snake(snake, direction, &food)){
                create_new_food(&food, snake);
                score++; // increment score
            }
            redraw = 1;     // redraw display
            break;

        // whenever a key is pressed
        case ALLEGRO_EVENT_KEY_DOWN:
            redraw = 1;

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
                // move snake in changed direction
                if (move_snake(snake, direction, &food)){
                    create_new_food(&food, snake);
                    score++; // increment score
                }
                direction = keypress;   // update current direction
                break;
            }
        }

        // break out of game loop if done with playing
        if (done){
            break;
        }

        // end game if snake collides with itself
        if (detect_snake_collisson(snake)){
            sleep(3);  // show final state for 5s
            break;
        }

        // redraw the display if redraw flag is set and event queue is empty
        if (redraw && al_is_event_queue_empty(queue)){
            al_clear_to_color(al_map_rgb(0, 0, 0));     // background color
            al_draw_text(font, al_map_rgb(255, 255, 255), 290, 660, 0, "Press ESC to exit..."); // display exit key
            al_draw_textf(font, al_map_rgb(255, 255, 255), 10, 10, 0, "Score: %i", score);      // display score
            al_draw_rectangle(40,40, 640, 640, al_map_rgb(0, 255, 255), 2);     // game boundary
            
            // if display grid flag is set
            if (display_grid){
                // for each 15px interval between 40 and 640
                for(int i = 55; i < 640; i += 15){
                    al_draw_line(40, i, 640, i, al_map_rgb(95, 158, 160), 1);    // draw horizontal line
                    al_draw_line(i, 40, i, 640, al_map_rgb(95, 158, 160), 1);    // draw vertical line
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

// loads start screen for the game. allows user to choose difficulty level and set option to display grid
int start_game_screen(
        ALLEGRO_EVENT_QUEUE* queue,
        ALLEGRO_TIMER* timer,
        ALLEGRO_DISPLAY* display,
        ALLEGRO_FONT* font,
        int* endgame_flag)
{
    int redraw = 0;         // set when display needs to be redrawn
    int difficulty = 3;     // default difficulty
    int rate_change = 0;    // set to 1 when difficulty is changed implying refresh rate has changed
    int display_grid = 0;   // display grid flag
    char grid[4];           // string version of display_grid to be drawn on display as YES or NO
    sprintf(grid, "NO");    // intializes display_grid flag string as NO
    int start_game = 0;     // indicates when player is ready to play

    ALLEGRO_EVENT event;    // stores events from queue
    // while user is not ready to play
    while (!start_game){

        // wait for an event
        al_wait_for_event(queue, &event);
        switch(event.type) {
        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            start_game = 1;
            *endgame_flag = 1;
            break;
        // redraw display at frame rate
        case ALLEGRO_EVENT_TIMER:
            redraw = 1;
            break;
        // respond to keypress
        case ALLEGRO_EVENT_KEY_DOWN:
            // adjust difficulty
            if(event.keyboard.keycode == ALLEGRO_KEY_UP){
                if(difficulty < 5){
                    difficulty += 1;
                    rate_change = 1;
                }
            }
            if(event.keyboard.keycode == ALLEGRO_KEY_DOWN){
                if(difficulty > 1){
                    difficulty -= 1;
                    rate_change = 1;
                }
            }

            // toggle display_grid flag
            if(event.keyboard.keycode == ALLEGRO_KEY_G){
                if (display_grid == 1){
                    display_grid = 0;
                    sprintf(grid, "NO");
                } else {
                    display_grid = 1;
                    sprintf(grid, "YES");
                }
            }

            // start game
            if (event.keyboard.keycode == ALLEGRO_KEY_SPACE){
                start_game = 1;
            }

            // redraw after keypress
            redraw = 1;
            break;
        }

        // update display frame rate by adjusting timer
        if (rate_change){
            float frame_rate;
            // maps difficulty values to frame rates
            switch(difficulty){
            case 1:
                frame_rate = 5.0;
                break;
            case 2:
                frame_rate = 10.0;
                break;
            case 3:
                frame_rate = 30.0;
                break;
            case 4:
                frame_rate = 40.0;
                break;
            case 5:
                frame_rate = 60.0;
                break;
            // default difficulty
            default:
                frame_rate = 30.0;
                break;
            }
            al_set_timer_speed(timer, 1.0 / frame_rate); // updates timer
            rate_change = 0;    // resets to indicate that that frame rate has been updated
        }

        // draws on display
        if (redraw && al_is_event_queue_empty(queue)){
            al_clear_to_color(al_map_rgb( 0, 0, 0));    // clear background to black

            // Start Menu
            al_draw_rectangle(100,150,580,400, al_map_rgb_f(0, 1, 1),2);
            al_draw_text(font, al_map_rgb_f(1, 1, 1), 290, 180, 0, "Play Snake!");
            al_draw_text(font, al_map_rgb_f(1, 1, 1), 250, 200, 0, "Press SPACE to start.");
            al_draw_textf(font, al_map_rgb_f(1, 1, 1), 286, 260, 0, "Difficulty: %i", difficulty);
            al_draw_textf(font, al_map_rgb_f(1, 1, 1), 270, 280, 0, "Display Grid: %s", grid);
            al_draw_text(font, al_map_rgb_f(1, 1, 1), 200, 340, 0, "Use UP and DOWN to change difficulty");
            al_draw_text(font, al_map_rgb_f(1, 1, 1), 195, 360, 0, "Press G to toggle display grid option");

            // send to display
            al_flip_display();
        }
    }
    return display_grid;    // return 1 if grid has to be displayed
}

void game_over_screen(
        ALLEGRO_EVENT_QUEUE* queue,
        ALLEGRO_TIMER* timer,
        ALLEGRO_DISPLAY* display,
        ALLEGRO_FONT* font,
        unsigned  int score)
        {

    int redraw = 0; // flag to redraw according to frame rate
    int done = 0;   // flag sets to 1 if a key is pressed

    // unless a key is pressed expressing desire to quit
    while(!done) {
        ALLEGRO_EVENT event;    // stores event
        al_wait_for_event(queue, &event);   // waits for event


        switch (event.type) {
            // redraw at frame rate
            case ALLEGRO_EVENT_TIMER:
                redraw = 1;
                break;

            // exit on any keypress or window close
            case ALLEGRO_EVENT_KEY_DOWN:
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = 1;
                break;
        }

        if (redraw && al_is_event_queue_empty(queue)){
            al_clear_to_color(al_map_rgb( 0, 0, 0));    // clear background to black

            // Start Menu
            al_draw_rectangle(100,150,580,400, al_map_rgb_f(0, 1, 1),2);
            al_draw_text(font, al_map_rgb_f(1, 1, 1), 300, 180, 0, "Game Over");
            al_draw_text(font, al_map_rgb_f(1, 1, 1), 270, 200, 0, "You played well!");
            al_draw_textf(font, al_map_rgb_f(1, 1, 1), 286, 260, 0, "Your score: %i", score);
            al_draw_text(font, al_map_rgb_f(1, 1, 1), 255, 360, 0, "Press ANY key to exit.");

            // send to display
            al_flip_display();
            redraw = 0; // do not redraw util timer goes off
        }
    }

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
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
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

    // constructs font to be used
    // returns NULL pointer if unable to create
    ALLEGRO_FONT* font = al_create_builtin_font();
    if (!(uintptr_t)font) {
        error("Cannot create font");
    }

    // Register events that would go into Event Queue
    al_register_event_source(queue, al_get_keyboard_event_source());    // Keyboard events
    al_register_event_source(queue, al_get_display_event_source(disp)); // Display events
    al_register_event_source(queue, al_get_timer_event_source(timer));  // Timer events

    unsigned int score = 0;     // initialize score to 0
    al_start_timer(timer);      // start timer to maintain constant frame rate accross gameplay

    int endgame_flag = 0;   // sets to 1 when close display is selected. allows to exit the program

    // main gameplay
    int display_grid = start_game_screen(queue, timer, disp, font, &endgame_flag);     // start screen
    if (!endgame_flag) {    // if window hasn't been closed
        score = play_snake(queue, timer, disp, font, display_grid, &endgame_flag);     // gameplay screen
    }
    if(!endgame_flag){      // if window hasn't been closed
        game_over_screen(queue, timer, disp, font, score);  // display score
    }

    // Destructors
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
}
