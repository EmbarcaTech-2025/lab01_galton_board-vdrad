#include "oled_display.h"

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

void oled_display_update_board(char board[ssd1306_width][ssd1306_height], uint16_t ball_count) {
    // Cria as variáveis para escrita do texto
    static uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);

    for (uint8_t i = 0; i < ssd1306_width; i++) {
        for (uint8_t j = 0; j < ssd1306_height; j++) {
            if(board[i][j] == '-') ssd1306_set_pixel(ssd, i, j, false);
            else ssd1306_set_pixel(ssd, i, j, true);
        }
    }
    
    char ball_count_str[6]; // Enough to hold "65535\0"
    snprintf(ball_count_str, sizeof(ball_count_str), "%u", ball_count);
    ssd1306_draw_string(ssd, 0, 0, ball_count_str);

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