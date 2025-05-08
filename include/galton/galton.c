#include "galton.h"
#include "pico/rand.h"  // Biblioteca contendo função para gerar números aleatórios
#include "include/oled_display/oled_display.h" // Biblioteca para uso do SSD1306, display OLED.
#include "include/oled_display/ssd1306_i2c.h" // Biblioteca para uso do SSD1306, display OLED.
#include <stdio.h>
#include <math.h>

char board[DISPLAY_WIDTH][DISPLAY_HEIGHT]; // '-' means empty space; 'b' means ball position; 'p' means pin position.
const uint8_t board_center   = 39;
const uint8_t lines          = 4;
uint8_t last_line_x_position[4];


/**
 * @brief Generates a random decision for the Galton board simulation.
 * This function uses a random number generator to decide whether to go LEFT or RIGHT.
 * @return A value of type `side`, either LEFT or RIGHT, based on the random number generated.
 */
side generate_random_side() {
    uint32_t number = get_rand_32();

    if (number >= UINT32_MAX/2) return LEFT;
    return RIGHT;
}

/**
 * @brief Clears the Galton Board display.
 * This function iterates through the entire Galton Board display
 * and resets each position to the default character '-'.
 * It ensures that the board is cleared and ready for a new simulation.
 */
void clear_board() {
    for (uint8_t i = 0; i < DISPLAY_WIDTH; i++) {
        for (uint8_t j = 0; j < DISPLAY_HEIGHT; j++) {
            board[i][j] = '-';
        }
    }
}

/**
 * @brief Draws a pin on the Galton board display.
 * This function marks the specified position and its surrounding points
 * on the board as part of a pin, represented by the character 'p'.
 * It ensures that the pin does not exceed the board boundaries.
 * 
 * @param x The x-coordinate of the pin's center.
 * @param y The y-coordinate of the pin's center.
 */
void draw_pin(int x, int y) {
    if ((x-1 < 0) || (y-1 < 0) || (x+1 >= DISPLAY_WIDTH) || (y+1 >= DISPLAY_HEIGHT)) return;

    board[x][y]     = 'p';
    board[x-1][y-1] = 'p';
    board[x-1][y+1] = 'p';
    board[x+1][y-1] = 'p';
    board[x+1][y+1] = 'p';
}

/**
 * @brief Generates the pins for the Galton board display.
 * This function creates a pattern of pins on the board, starting from an initial position
 * and spacing them out based on the specified gap and number of lines.
 * It ensures that the pins are drawn symmetrically and within the board boundaries.
 */
void generate_board_pins() {
    // Board parameters
    const uint8_t initial_x   = board_center;
    const uint8_t initial_y   = 25;
    const uint8_t gap         = 10;

    // Draw pins on the display
    for (uint8_t i = 0; i < lines; i++) {
        for (int8_t j = -i; j <= i; j += 2) {
            draw_pin(initial_x + j*gap, initial_y + i*gap);
        }
    }

    for (uint8_t i = 0; i < lines; i++) {
        last_line_x_position[i] = (initial_x - (lines - 1) * gap) + i*2*gap;
    }
}

void draw_ball(ball_struct *ball) {
    if ((ball->x_position-2 < 0) || (ball->y_position-2 < 0) || (ball->x_position+2 >= DISPLAY_WIDTH) || (ball->y_position+2 >= DISPLAY_HEIGHT)) return;
    ball->collision = false;

    for (int8_t i = -1; i < 2; i++) {
        if (
            board[ball->x_position+i][ball->y_position-2] == 'p' ||
            board[ball->x_position+i][ball->y_position+2] == 'p' ||
            board[ball->x_position-2][ball->y_position+i] == 'p' ||
            board[ball->x_position+2][ball->y_position+i] == 'p' 
        ) {
            ball->collision = true;
        } else {
            board[ball->x_position+i][ball->y_position-2] = 'b';
            board[ball->x_position+i][ball->y_position+2] = 'b';
            board[ball->x_position-2][ball->y_position+i] = 'b';
            board[ball->x_position+2][ball->y_position+i] = 'b';
        }
    }
}

void update_board_matrix(ball_struct *ball[NUMBER_OF_BALLS], uint16_t *ball_count) {
    clear_board();
    generate_board_pins();
    
    for (uint8_t i = 0; i < NUMBER_OF_BALLS; i++) {   
        draw_ball(ball[i]);
        if (ball[i]->collision) {
            side random_side = generate_random_side();
            
            // sort a random integer between 0 and 10 to be the horizontal shift
            int8_t horizontal_shift = 5 + round(((float)get_rand_32()/UINT32_MAX)*10.0);
            if (random_side == LEFT) horizontal_shift *= -1;
            
            ball[i]->x_position += horizontal_shift;
            ball[i]->drop_location = NONE;
            continue;
        }
        
        if (ball[i]->y_position < DISPLAY_HEIGHT - 1) {
            ball[i]->y_position++;
            ball[i]->drop_location = NONE;
            continue;
        }
        
        if (ball[i]->x_position < last_line_x_position[0]) ball[i]->drop_location = ZONE_1;
        if (ball[i]->x_position >= last_line_x_position[0] && ball[i]->x_position < last_line_x_position[1]) ball[i]->drop_location = ZONE_2;
        if (ball[i]->x_position >= last_line_x_position[1] && ball[i]->x_position < last_line_x_position[2]) ball[i]->drop_location = ZONE_3;
        if (ball[i]->x_position >= last_line_x_position[2] && ball[i]->x_position < last_line_x_position[3]) ball[i]->drop_location = ZONE_4;
        if (ball[i]->x_position >= last_line_x_position[3] && ball[i]->x_position < last_line_x_position[4]) ball[i]->drop_location = ZONE_5;
        (*ball_count)++;
    }
    oled_display_update_board(board, (*ball_count));
}

void calculate_histogram(ball_struct *ball[NUMBER_OF_BALLS]) {
    for (uint8_t i = 0; i < NUMBER_OF_BALLS; i++) {  
        if (ball[i]->drop_location) {
            ball[i]->finished_moving = true;
        }
    }
}

void board_init() {
    ball_struct ball = {
        .x_position     = board_center,
        .y_position     = 5,
        .collision      = false,
        .drop_location  = NONE
    };

    ball_struct balls[NUMBER_OF_BALLS];
    ball_struct *ball_pointers[NUMBER_OF_BALLS];
    for (uint8_t i = 0; i < NUMBER_OF_BALLS; i++) {
        balls[i].x_position = board_center;
        balls[i].y_position = 5 - 15*i;
        balls[i].collision = false;
        balls[i].drop_location = NONE;
        ball_pointers[i] = &balls[i];
    }

    clear_board();
    while (true) {
        uint16_t ball_count = 0;
        update_board_matrix(ball_pointers, &ball_count);
        calculate_histogram(ball_pointers);
        printf("Ball Count: %d \n", ball_count);
        // sleep_ms(200);
        // if (balls[2].drop_location) {
        //     reset_balls(ball_pointers);
        //     printf("Drop Zone: %d \n", balls[2].drop_location);
        // }
    }
}