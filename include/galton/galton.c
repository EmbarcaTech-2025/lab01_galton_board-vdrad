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

void draw_ball(ball_struct *ball, bool *collision) {
    if ((ball->x_position-2 < 0) || (ball->y_position-2 < 0) || (ball->x_position+2 >= DISPLAY_WIDTH) || (ball->y_position+2 >= DISPLAY_HEIGHT)) return;
    *collision = false;

    for (int8_t i = -1; i < 2; i++) {
        if (
            board[ball->x_position+i][ball->y_position-2] == 'p' ||
            board[ball->x_position+i][ball->y_position+2] == 'p' ||
            board[ball->x_position-2][ball->y_position+i] == 'p' ||
            board[ball->x_position+2][ball->y_position+i] == 'p' 
        ) {
            *collision = true;
        } else {
            board[ball->x_position+i][ball->y_position-2] = 'b';
            board[ball->x_position+i][ball->y_position+2] = 'b';
            board[ball->x_position-2][ball->y_position+i] = 'b';
            board[ball->x_position+2][ball->y_position+i] = 'b';
        }
    }
}

drop_zone update_board_matrix(ball_struct *ball) {
    bool collision = false;

    clear_board();
    generate_board_pins();
    draw_ball(ball, &collision);
    oled_display_update_board(board);

    if (collision) {
        side random_side = generate_random_side();

        // sort a random integer between 0 and 10 to be the horizontal shift
        int8_t horizontal_shift = 5 + round(((float)get_rand_32()/UINT32_MAX)*5.0);
        if (random_side == LEFT) horizontal_shift *= -1;

        ball->x_position += horizontal_shift;
        return NONE;
    }

    if (ball->y_position < DISPLAY_HEIGHT - 1) {
        ball->y_position++;
        return NONE;
    }

    if (ball->x_position < last_line_x_position[0]) return ZONE_1;
    if (ball->x_position >= last_line_x_position[0] && ball->x_position < last_line_x_position[1]) return ZONE_2;
    if (ball->x_position >= last_line_x_position[1] && ball->x_position < last_line_x_position[2]) return ZONE_3;
    if (ball->x_position >= last_line_x_position[2] && ball->x_position < last_line_x_position[3]) return ZONE_4;
    if (ball->x_position >= last_line_x_position[3] && ball->x_position < last_line_x_position[4]) return ZONE_5;
}

void board_init() {
    ball_struct ball = {
        .x_position     = board_center,
        .y_position     = 5,
        .drop_location  = NONE
    };

    clear_board();
    while (true) {
        drop_zone current_drop_zone = update_board_matrix(&ball);
        if (current_drop_zone) {
            ball.y_position = 5;
            ball.x_position = board_center;
            printf("Drop Zone: %d \n", current_drop_zone);
        }

        // while (ball.y_position < DISPLAY_HEIGHT - 1) {
        //     // printf("Ball position Y: %d \n", ball.y_position);
        //     // ball.y_position++;
        //     drop_zone current_drop_zone = update_board_matrix(&ball);
        //     if (current_drop_zone) printf("Drop Zone: %d \n", current_drop_zone);
        //     sleep_ms(20);
        // }

        // ball.y_position = 5;
        // ball.x_position = board_center;
    }

}