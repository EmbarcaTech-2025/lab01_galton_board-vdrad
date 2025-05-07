#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"


#include "include/oled_display/oled_display.h" // Biblioteca para uso do SSD1306, display OLED.
#include "include/galton/galton.h"             // Biblioteca com funções relacionadas ao projeto da Galton Board

int main() {
    stdio_init_all();
    oled_display_init();

    board_init();
    while (true) {

    }

    return 0;
}
