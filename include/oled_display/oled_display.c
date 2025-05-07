#include "oled_display.h"

#include <stdio.h>                          // Biblioteca para as funções gerais de pino e UART.
#include <string.h>                         // Biblioteca para lidar com variáveis do tipo string.
#include <ctype.h>                          // Biblioteca para lidar com caracteres ASCII.
#include "pico/stdlib.h"                    // Biblioteca geral com códigos pertinentes à RP2040.
#include "pico/binary_info.h"               // Biblioteca com algumas informações binárias da RP2040.
#include "hardware/i2c.h"                   // Biblioteca da RP2040 para comunicação I2C.
#include "include/oled_display/ssd1306.h"       // Biblioteca para controle do display OLED da BitDogLab.
#include "include/oled_display/ssd1306_i2c.h"   // Biblioteca para controle do display OLED da BitDogLab.

// Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
struct render_area frame_area = {
    start_column : 0,
    end_column : ssd1306_width - 1,
    start_page : 0,
    end_page : ssd1306_n_pages - 1
};

/**
 * Inicializa o display OLED da BitDogLab.
 */
void oled_display_init() {
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);

    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
    
    ssd1306_init();
    // Preparar área de renderização para o display (ssd1306_width pixels por ssd1306_n_pages páginas)
    calculate_render_area_buffer_length(&frame_area);
}

/**
 * Escreve o texto informado no Display OLED.
 * @param text      o texto a ser escrito
 * @param n_lines   o número de linhas no texto a ser escrito
 * @param inital_y  a altura inicial do texto a ser escrito
 */
void oled_display_write(char *text[], uint8_t n_lines, int16_t initial_y) {
    // Cria as variáveis para escrita do texto
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);

    // Parte do código para exibir a mensagem no display.
    int16_t y = initial_y;
    for (uint i = 0; i < n_lines; i++) {
        ssd1306_draw_string(ssd, 5, y, text[i]);
        y += 8;
    }
    render_on_display(ssd, &frame_area);
}

void oled_display_draw_pin(uint8_t *ssd, int x, int y) {
    ssd1306_set_pixel(ssd, x, y, true);
    ssd1306_set_pixel(ssd, x-1, y-1, true);
    ssd1306_set_pixel(ssd, x-1, y+1, true);
    ssd1306_set_pixel(ssd, x+1, y-1, true);
    ssd1306_set_pixel(ssd, x+1, y+1, true);
}

void oled_display_draw_ball(uint8_t *ssd, int x, int y) {
    ssd1306_set_pixel(ssd, x, y-2, true);
    ssd1306_set_pixel(ssd, x-1, y-2, true);
    ssd1306_set_pixel(ssd, x+1, y-2, true);

    ssd1306_set_pixel(ssd, x, y+2, true);
    ssd1306_set_pixel(ssd, x-1, y+2, true);
    ssd1306_set_pixel(ssd, x+1, y+2, true);

    ssd1306_set_pixel(ssd, x-2, y, true);
    ssd1306_set_pixel(ssd, x-2, y+1, true);
    ssd1306_set_pixel(ssd, x-2, y-1, true);

    ssd1306_set_pixel(ssd, x+2, y, true);
    ssd1306_set_pixel(ssd, x+2, y+1, true);
    ssd1306_set_pixel(ssd, x+2, y-1, true);
}

void oled_display_draw_board(int ball_x, int ball_y) {
    // Cria as variáveis para escrita do texto
    static uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);

    // Parâmetros para desenhar a Galton Board
    uint8_t x_ini = 39;
    uint8_t y_ini = 30;
    uint8_t step = 10;

    // Desenha os pinos da Galton Board
    for (uint8_t i = 0; i < 4; i++) {
        oled_display_draw_pin(ssd, x_ini + i*step, y_ini + i*step);
        oled_display_draw_pin(ssd, x_ini - i*step, y_ini + i*step);

        if (i%2 == 0) {
            oled_display_draw_pin(ssd, x_ini, y_ini + i*step);
        } else {
            oled_display_draw_pin(ssd, x_ini + (i-2)*step, y_ini + i*step);
            oled_display_draw_pin(ssd, x_ini - (i-2)*step, y_ini + i*step);
        }
    }

    oled_display_draw_ball(ssd, ball_x, ball_y);

    ssd1306_draw_line(ssd, 80, 0, 80, 64, true); // À esquerda da linha: Galton Board. À direita: Histograma.
    render_on_display(ssd, &frame_area);
}

/**
 * Limpa a tela do display OLED.
 */
void oled_display_clear() {
    char *text[] = {""};

    oled_display_write(text, count_of(text), 0);
}

/**
 * Exibe uma mensagem de teste na tela do Display OLED.
 */
void oled_display_validate() {
    char *text[] = {
        "ReMedicine",
        "EMBARCATECH"
    };

    oled_display_write(text, count_of(text), 8);
}