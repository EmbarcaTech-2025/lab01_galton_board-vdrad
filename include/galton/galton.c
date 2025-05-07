#include "galton.h"
#include "pico/rand.h"  // Biblioteca contendo função para gerar números aleatórios

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