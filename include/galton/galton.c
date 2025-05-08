#include "galton.h"
#include "pico/rand.h"  // Library for generating random numbers
#include "include/oled_display/oled_display.h" // Library for SSD1306 OLED display
#include "include/oled_display/ssd1306_i2c.h" // Library for SSD1306 OLED display
#include <stdio.h>
#include <math.h>

char board[DISPLAY_WIDTH][DISPLAY_HEIGHT]; // '-' means empty space; 'b' means ball position; 'p' means pin position.
const uint8_t board_center   = 39; // Center position of the board
const uint8_t lines          = 4;  // Number of lines of pins
uint8_t last_line_x_position[4];   // Stores the x-coordinates of the last line of pins

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
    const uint8_t initial_x   = board_center; // Initial x-coordinate for the pins
    const uint8_t initial_y   = 25;           // Initial y-coordinate for the pins
    const uint8_t gap         = 10;           // Gap between pins

    // Draw pins on the display
    for (uint8_t i = 0; i < lines; i++) {
        for (int8_t j = -i; j <= i; j += 2) {
            draw_pin(initial_x + j*gap, initial_y + i*gap);
        }
    }

    // Store the x-coordinates of the last line of pins
    for (uint8_t i = 0; i < lines; i++) {
        last_line_x_position[i] = (initial_x - (lines - 1) * gap) + i*2*gap;
    }
}

/**
 * @brief Draws a ball on the Galton board display.
 * This function updates the board matrix to represent the ball's position.
 * It also checks for collisions with pins and updates the ball's collision status.
 * 
 * @param ball Pointer to the ball structure containing its position and collision status.
 */
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

/**
 * @brief Calculates and updates the histogram for the Galton board simulation.
 * This function computes the distribution of balls in different zones and updates
 * the board matrix to display the histogram.
 * 
 * @param ball Array of pointers to ball structures.
 * @param ball_count Total number of balls dropped.
 */
void calculate_histogram(ball_struct *ball[NUMBER_OF_BALLS], uint16_t ball_count) {
    uint16_t zone_counts[5]     = {0,  0,  0,  0,   0}; // Counts for each zone
    uint8_t zone_positions[5]   = {73, 84, 95, 106, 117}; // x-coordinates for histogram zones

    // Count balls in each zone
    for (uint8_t i = 0; i < NUMBER_OF_BALLS; i++) {
        switch(ball[i]->drop_location) {
            case ZONE_1:
                zone_counts[0]++;
                break;
            case ZONE_2:
                zone_counts[1]++;
                break;
            case ZONE_3:
                zone_counts[2]++;
                break;
            case ZONE_4:
                zone_counts[3]++;
                break;
            case ZONE_5:
                zone_counts[4]++;
                break;
            default: 
                break;
        }
    }

    // Update the histogram on the board
    for (uint8_t i = 0; i < 5; i++) {
        if (ball_count > 0) {
            zone_counts[i] = (uint16_t)round((float)(DISPLAY_HEIGHT + 40) * (float)zone_counts[i] / (float)ball_count);
            printf("Zone Count [%d]: %d \n", i, zone_counts[i]);

            for (uint8_t k = 127; k > 127 - zone_counts[i]; k--) {
                for (uint8_t j = zone_positions[i]; j < zone_positions[i] + 10; j++) {
                    board[j][k] = 'h';
                }   
            }
        }
    }
    printf("\n\n");
}

/**
 * @brief Updates the Galton board matrix with the current state of the simulation.
 * This function clears the board, generates pins, updates ball positions, and calculates
 * the histogram based on the ball distribution.
 * 
 * @param ball Array of pointers to ball structures.
 * @param ball_count Pointer to the total number of balls dropped.
 */
void update_board_matrix(ball_struct *ball[NUMBER_OF_BALLS], uint16_t *ball_count) {
    clear_board();
    generate_board_pins();
    
    for (uint8_t i = 0; i < NUMBER_OF_BALLS; i++) {   
        draw_ball(ball[i]);
        if (ball[i]->collision) {
            side random_side = generate_random_side();
            
            // Sort a random integer between 0 and 10 to be the horizontal shift
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
        
        // Determine the drop location based on x_position
        if (ball[i]->x_position < last_line_x_position[0]) ball[i]->drop_location = ZONE_1;
        if (ball[i]->x_position >= last_line_x_position[0] && ball[i]->x_position < last_line_x_position[1]) ball[i]->drop_location = ZONE_2;
        if (ball[i]->x_position >= last_line_x_position[1] && ball[i]->x_position < last_line_x_position[2]) ball[i]->drop_location = ZONE_3;
        if (ball[i]->x_position >= last_line_x_position[2] && ball[i]->x_position < last_line_x_position[3]) ball[i]->drop_location = ZONE_4;
        if (ball[i]->x_position >= last_line_x_position[3] && ball[i]->x_position < last_line_x_position[4]) ball[i]->drop_location = ZONE_5;
        (*ball_count)++;
    }
    calculate_histogram(ball, (*ball_count));
    oled_display_update_board(board, (*ball_count));
}

/**
 * @brief Initializes the Galton board simulation.
 * This function sets up the initial state of the balls and continuously updates
 * the board matrix to simulate the Galton board.
 */
void board_init() {
    ball_struct balls[NUMBER_OF_BALLS]; // Array of ball structures
    ball_struct *ball_pointers[NUMBER_OF_BALLS]; // Array of pointers to ball structures

    // Initialize balls with starting positions
    for (uint8_t i = 0; i < NUMBER_OF_BALLS; i++) {
        balls[i].x_position = board_center;
        balls[i].y_position = 5 - 15*i;
        balls[i].drop_location = NONE;
        balls[i].collision = false;
        ball_pointers[i] = &balls[i];
    }

    clear_board();
    while (true) {
        uint16_t ball_count = 0;
        update_board_matrix(ball_pointers, &ball_count);
    }
}