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
    oled_display_draw_board();
    // static uint8_t ssd[ssd1306_buffer_length];
    // // memset(ssd, 0, ssd1306_buffer_length);

    // // oled_display_draw_line(80,0,80,64); // À esquerda da linha: Galton Board. À direita: Histograma.
    // draw_board(ssd);
    // render_on_display(ssd, &frame_area);
    // oled_display_update();
    // // oled_display_clear();
    // oled_display_draw_pixel(40, 40);
    // oled_display_draw_pixel(81, 81);
    // oled_display_draw_pixel(82, 82);
    // oled_display_draw_pixel(83, 83);

    // char *text[10] = {
    //     "            O            ",
    //     "X  X   X   X   X   X   X",
    //     "C",
    //     "D",
    //     "E",
    //     "F",
    //     "G",
    //     "H",
    //     "I",
    //     "J",
    // };
    // oled_display_write(text, count_of(text), 0);
}