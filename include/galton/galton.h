#ifndef __GALTON_H__ // Caso já tenha sido declarada em algum outro lugar, não declare novamente.
#define __GALTON_H__

#include <stdint.h>
#include "pico/stdlib.h"

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

#define NUMBER_OF_BALLS 200
typedef enum {
    LEFT,
    RIGHT
} side;

typedef enum {
    NONE,
    ZONE_1,
    ZONE_2,
    ZONE_3,
    ZONE_4,
    ZONE_5,
} drop_zone;

typedef struct {
    int16_t x_position;
    int16_t y_position;
    drop_zone drop_location;
    bool collision;
    bool finished_moving;
} ball_struct;

side generate_random_side();
void board_init();
#endif