#ifndef __PINOUT_H__ // Caso já tenha sido declarada em algum outro lugar, não declare novamente.
#define __PINOUT_H__

// Pinos da Matriz de LEDs RGB
#define RGB_MATRIX_PIN 7
#define RGB_MATRIX_COUNT 25

// Pinos I2C
#define I2C_SDA_PIN 14
#define I2C_SCL_PIN 15

// Pinos do Buzzer 
#define LEFT_BUZZER_PIN 21   
#define RIGHT_BUZZER_PIN 10   

// Pinos dos botões
#define A_BUTTON_PIN 5
#define B_BUTTON_PIN 6

// Pino do Microfone
#define MICROPHONE_CHANNEL 2
#define MICROPHONE_PIN (26 + MICROPHONE_CHANNEL)

#endif