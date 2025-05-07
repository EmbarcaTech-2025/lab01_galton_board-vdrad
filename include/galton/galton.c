#include "galton.h"
#include "pico/rand.h"  // Biblioteca contendo função para gerar números aleatórios
#include "include/oled_display/oled_display.h" // Biblioteca para uso do SSD1306, display OLED.
#include "include/oled_display/ssd1306_i2c.h" // Biblioteca para uso do SSD1306, display OLED.

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

void board_init() {
    ball_struct ball = {
        .x_position = 39,
        .y_position = 5
    };

    oled_display_draw_board(ball.x_position, ball.y_position);
    while (ball.y_position < 128) {
        ball.y_position++;
        oled_display_draw_board(ball.x_position, ball.y_position);
        sleep_ms(20);
    }
}