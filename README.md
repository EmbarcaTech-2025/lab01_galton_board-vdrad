
# Projetos de Sistemas Embarcados - EmbarcaTech 2025

Autor: **Victor de Andrade Gomes**

Curso: Residência Tecnológica em Sistemas Embarcados

Instituição: EmbarcaTech - HBr

Campinas, 06 de maio de 2025

---

## Descrição do Projeto

O Galton Board é um dispositivo que demonstra a distribuição normal por meio de esferas que colidem com pinos dispostos geometricamente. Este projeto simula o comportamento do Galton Board em um display OLED da BitDogLab, utilizando o conceito de Digital Twin. A simulação permite observar a distribuição das esferas e compará-la com a teoria estatística.

---

## Objetivo

Criar uma simulação interativa de um Galton Board no display OLED, utilizando o microcontrolador Raspberry Pi Pico. O objetivo é demonstrar a distribuição normal das esferas ao final do tabuleiro e visualizar os resultados em tempo real.

---

## Funcionalidades Implementadas

### 1. Geração de Aleatoriedade
A função `generate_random_side` utiliza a biblioteca `pico/rand.h` para decidir aleatoriamente se uma esfera deve desviar para a esquerda ou para a direita. A decisão é baseada em um número aleatório de 32 bits.

```c
side generate_random_side() {
    uint32_t number = get_rand_32();
    return (number >= UINT32_MAX / 2) ? LEFT : RIGHT;
}
```

### 2. Representação do Tabuleiro
O tabuleiro é representado por uma matriz bidimensional, onde:
- `'-'` indica espaço vazio.
- `'p'` indica a posição de um pino.
- `'b'` indica a posição de uma esfera.
- `'h'` indica a altura de uma barra no histograma.

### 3. Geração de Pinos
A função `generate_board_pins` cria um padrão geométrico de pinos no tabuleiro, garantindo simetria e espaçamento adequado.

```c
void generate_board_pins() {
    const uint8_t initial_x = board_center;
    const uint8_t initial_y = 25;
    const uint8_t gap = 10;

    for (uint8_t i = 0; i < lines; i++) {
        for (int8_t j = -i; j <= i; j += 2) {
            draw_pin(initial_x + j * gap, initial_y + i * gap);
        }
    }
}
```

### 4. Simulação da Queda das Esferas
As esferas percorrem o tabuleiro, desviando para a esquerda ou direita com base na função de aleatoriedade. Ao atingir a base, a posição final é registrada para análise.

```c
void update_board_matrix(ball_struct *ball[NUMBER_OF_BALLS], uint16_t *ball_count) {
    clear_board();
    generate_board_pins();
    for (uint8_t i = 0; i < NUMBER_OF_BALLS; i++) {
        draw_ball(ball[i]);
        // Lógica de colisão e movimentação
    }
    calculate_histogram(ball, (*ball_count));
    oled_display_update_board(board, (*ball_count));
}
```

### 5. Renderização no Display OLED
O estado do tabuleiro é atualizado em tempo real no display OLED, utilizando a biblioteca `ssd1306_i2c.h`. O histograma é gerado para representar a distribuição final das esferas.

```c
void calculate_histogram(ball_struct *ball[NUMBER_OF_BALLS], uint16_t ball_count) {
    uint16_t zone_counts[5] = {0, 0, 0, 0, 0};
    // Lógica para calcular e renderizar o histograma
}
```

---

## Resultados Obtidos

Após a simulação de 1.000 esferas, a distribuição final apresentou um padrão semelhante à curva de distribuição normal, validando a teoria estatística por trás do Galton Board.

---

## Estrutura do Código

O projeto está organizado nos seguintes arquivos:

- **`galton.c`**: Contém a lógica principal da simulação, incluindo a geração de pinos, movimentação das esferas e renderização no display.
- **`galton.h`**: Define as estruturas de dados, constantes e protótipos de funções.
- **Bibliotecas Externas**:
  - `pico/rand.h`: Para geração de números aleatórios.
  - `ssd1306_i2c.h`: Para controle do display OLED.

---

## Como Executar

1. Configure o ambiente de desenvolvimento para o Raspberry Pi Pico.
2. Compile o código utilizando o Pico SDK.
3. Carregue o firmware no microcontrolador.
4. Conecte o display OLED e observe a simulação em tempo real.

---

## 📜 Licença

Este projeto está licenciado sob a **MIT License**.

