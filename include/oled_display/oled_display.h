#ifndef __OLED_DISPLAY_H__ // Caso já tenha sido declarada em algum outro lugar, não declare novamente.
#define __OLED_DISPLAY_H__

#include <stdio.h>                          // Biblioteca para as funções gerais de pino e UART.
#include <string.h>                         // Biblioteca para lidar com variáveis do tipo string.
#include <ctype.h>                          // Biblioteca para lidar com caracteres ASCII.
#include "pico/stdlib.h"                    // Biblioteca geral com códigos pertinentes à RP2040.
#include "pico/binary_info.h"               // Biblioteca com algumas informações binárias da RP2040.
#include "hardware/i2c.h"                   // Biblioteca da RP2040 para comunicação I2C.
#include "include/pinout.h"
#include "include/oled_display/ssd1306.h"       // Biblioteca para controle do display OLED da BitDogLab.
#include "include/oled_display/ssd1306_i2c.h"   // Biblioteca para controle do display OLED da BitDogLab.

void oled_display_init();
void oled_display_clear();
void oled_display_write(char *text[], uint8_t n_lines, int16_t initial_y);
void oled_display_draw_ball(uint8_t *ssd, int x, int y);
void oled_display_draw_board(int ball_x, int ball_y);
void oled_display_update_board(char board[ssd1306_width][ssd1306_height]);
void oled_display_validate();

#endif