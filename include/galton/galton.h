#ifndef __GALTON_H__ // Caso já tenha sido declarada em algum outro lugar, não declare novamente.
#define __GALTON_H__

#include <stdint.h>

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
typedef enum {
    LEFT,
    RIGHT
} side;

typedef struct {
    int8_t x_position;
    int8_t y_position;
} ball_struct;

side generate_random_side();
void board_init();
#endif