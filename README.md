
# Projetos de Sistemas Embarcados - EmbarcaTech 2025

Autor: **Victor de Andrade Gomes**

Curso: Residência Tecnológica em Sistemas Embarcados

Instituição: EmbarcaTech - HBr

Campinas, 06 de maio de 2025

---

## Descrição do Projeto
O Galton Board consiste em uma série de pinos dispostos geometricamente. Esferas são liberadas de um ponto central superior e, à medida que caem, colidem com os pinos e se desviam aleatoriamente para a esquerda ou para a direita. Esse processo continua até que as esferas alcancem a base do tabuleiro, onde se acumulam em compartimentos distintos.

## Objetivo
Simular uma Galton Board no display OLED da BitDogLab, usando uma abordagem de Digital Twin. Isso significa construir um modelo computacional que se comporte de maneira semelhante ao dispositivo físico, permitindo observar a distribuição das "bolas" ao final do tabuleiro — e compará-las com a teoria da distribuição normal.

## Etapas do Desenvolvimento

### Aleatoriedade Binária

A função de aleatoriedade binária foi criada utilizando a biblioteca `rand`, interna do Pico SDK. Dela, extrai-se um número aleatório em formato `uint32_t`. Caso ele seja maior que a metade da faixa, decide-se pelo lado esquerdo. Caso contrário, decide-se pelo lado direito. A função foi testada em 10000 casos e produziu distribuição satisfatória (aproximadamente 50% para cada lado).

````c
side generate_random_side() {
    uint32_t number = get_rand_32();

    if (number >= UINT32_MAX/2) return LEFT;
    return RIGHT;
}
````

---

## 📜 Licença
MIT License - MIT GPL-3.0.

