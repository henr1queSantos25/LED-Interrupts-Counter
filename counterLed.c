#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"

// ARQUIVO .pio
#include "counterLed.pio.h"

// NÚMERO DE LEDS
#define NUM_PIXELS 25

// PINO DA MATRIZ DE LED
#define OUT_PIN 7

#define button 6
#define reset 5

double numeros[10][NUM_PIXELS] = {
    // Número 0
    {0, 0, 1, 1, 1,
     1, 0, 1, 0, 0,
     0, 0, 1, 0, 1,
     1, 0, 1, 0, 0,
     0, 0, 1, 1, 1},

    // Número 1
    {0, 0, 0, 0, 1,
     1, 0, 0, 0, 0,
     0, 0, 0, 0, 1,
     1, 0, 0, 0, 0,
     0, 0, 0, 0, 1},

    // Número 2
    {0, 0, 1, 1, 1,
     1, 0, 0, 0, 0,
     0, 0, 1, 1, 1,
     0, 0, 1, 0, 0,
     0, 0, 1, 1, 1},

    // Número 3
    {0, 0, 1, 1, 1,
     1, 0, 0, 0, 0,
     0, 0, 1, 1, 1,
     1, 0, 0, 0, 0,
     0, 0, 1, 1, 1},

    // Número 4
    {0, 0, 1, 0, 1,
     1, 0, 1, 0, 0,
     0, 0, 1, 1, 1,
     1, 0, 0, 0, 0,
     0, 0, 0, 0, 1},

    // Número 5
    {0, 0, 1, 1, 1,
     0, 0, 1, 0, 0,
     0, 0, 1, 1, 1,
     1, 0, 0, 0, 0,
     0, 0, 1, 1, 1},

    // Número 6
    {0, 0, 1, 1, 1,
     0, 0, 1, 0, 0,
     0, 0, 1, 1, 1,
     1, 0, 1, 0, 0,
     0, 0, 1, 1, 1},

    // Número 7
    {0, 0, 1, 1, 1,
     1, 0, 0, 0, 0,
     0, 0, 0, 0, 1,
     1, 0, 0, 0, 0,
     0, 0, 0, 0, 1},

    // Número 8
    {0, 0, 1, 1, 1,
     1, 0, 1, 0, 0,
     0, 0, 1, 1, 1,
     1, 0, 1, 0, 0,
     0, 0, 1, 1, 1},

    // Número 9
    {0, 0, 1, 1, 1,
     1, 0, 1, 0, 0,
     0, 0, 1, 1, 1,
     1, 0, 0, 0, 0,
     0, 0, 1, 1, 1}};

// rotina para definição da intensidade de cores do led
uint32_t matrix_rgb(double r, double g, double b)
{
    unsigned char R, G, B;
    R = r * 255;
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}
// rotina para acionar a matrix de leds - ws2812b
void desenho_pio(double desenho[10][25], int digit, uint32_t valor_led, PIO pio, uint sm)
{

    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        if(desenho[digit][24 - i]){
            valor_led = matrix_rgb(0, 0, 0.1);
        }
        else{
            valor_led = matrix_rgb(0, 0, 0);
        }
        put_pixel(valor_led);
    }

}



// função principal
int main()
{
    PIO pio = pio0;
    bool ok;
    uint16_t i;
    uint32_t valor_led;
    double r = 0.0, b = 0.0, g = 0.0;

    // coloca a frequência de clock para 128 MHz, facilitando a divisão pelo clock
    ok = set_sys_clock_khz(128000, false);

    // Inicializa todos os códigos stdio padrão que estão ligados ao binário.
    stdio_init_all();

    printf("iniciando a transmissão PIO");
    if (ok)
        printf("clock set to %ld\n", clock_get_hz(clk_sys));

    // configurações da PIO
    uint offset = pio_add_program(pio, &counterLed_program);
    uint sm = pio_claim_unused_sm(pio, true);
    counterLed_program_init(pio, sm, offset, OUT_PIN);

    // inicializar o botão de interrupção - GPIO5
    gpio_init(button);
    gpio_set_dir(button, GPIO_IN);
    gpio_pull_up(button);

    gpio_init(reset);
    gpio_set_dir(reset, GPIO_IN);
    gpio_pull_up(reset);


    while (true)
    {

        if(!gpio_get(button)){
            desenho_pio(numeros, 2, valor_led, pio, sm);
        }
        if(!gpio_get(reset)){
            reset_usb_boot(0, 0);
        }
        
        
    }
}
