#ifndef __GALTON_H__ // Caso já tenha sido declarada em algum outro lugar, não declare novamente.
#define __GALTON_H__

typedef enum {
    LEFT,
    RIGHT
} side;

typedef struct {
    int8_t x_position;
    int8_t y_position;
} ball;

side generate_random_side();
#endif