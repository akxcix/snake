#ifndef BASE_H
#define BASE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

#define LEFT -1
#define RIGHT 1
#define UP 2
#define DOWN -2

typedef struct point {
    int x;
    int y;    
} point;

#endif