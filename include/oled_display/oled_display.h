#ifndef __OLED_DISPLAY_H__ // Caso já tenha sido declarada em algum outro lugar, não declare novamente.
#define __OLED_DISPLAY_H__

#include "pico/stdlib.h"        // Biblioteca geral com códigos pertinentes à RP2040.
#include "inc/pinout.h"         // Biblioteca contendo a relação dos pinos da RP2040 utilizados no projeto.

void oled_display_init();
void oled_display_write(char *text[], uint8_t n_lines, int16_t initial_y);
void oled_display_validate();

#endif