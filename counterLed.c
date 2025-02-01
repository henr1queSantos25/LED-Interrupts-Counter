#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"
#include "hardware/timer.h"

// ARQUIVO .pio
#include "counterLed.pio.h"

// PINO DA MATRIZ DE LED
#define OUT_PIN 7

// definição dos pinos dos botões e do led vermelho
#define button_0 5
#define button_1 6
#define button_2 22
#define LED_RED 13

// VARIÁVEIS VOLÁTEIS
int volatile number = 0; // número que vai aparecer no led 5x5
uint32_t volatile last_time = 0; // variável que auxilia no debounce

// FUNÇÕES QUE ESTÃO DO ARQUIVO "functions.c"
extern void drawMatrix(int digit); // desenha cada número na matriz 5x5
extern void changeMatrixColor();   // mudar a cor dos leds na matriz 5x5
extern uint32_t urgb_u32(double r, double g, double b); // função que retorna a cor do led
extern void put_pixel(uint32_t pixel_grb); // liga os leds necessários para formar um número

// PROTÓTIPOS
void gpio_irq_handler(uint gpio, uint32_t events);
void initButtons();
void initLed();


int main()
{
    // Inicializa todos os códigos stdio padrão
    stdio_init_all();

    // configurações da PIO
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &counterLed_program);
    uint sm = pio_claim_unused_sm(pio, true);
    counterLed_program_init(pio, sm, offset, OUT_PIN);

    // INICIALIZAÇÃO DOS BUTTONS
    initButtons();

    // INICIALIZAÇÃO DO LED
    initLed();

    // Configuração das interrupções com callback
    gpio_set_irq_enabled_with_callback(button_0, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(button_1, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(button_2, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    while (true)
    {
        // LOOP PRINCIPAL
        gpio_put(LED_RED, true);
        sleep_ms(100);
        gpio_put(LED_RED, false);
        sleep_ms(100);
    }
}


// FUNÇÃO CHAMADA PELA INTERRUPÇÃO
void gpio_irq_handler(uint gpio, uint32_t events)
{
    // Obtém o tempo atual em microssegundos para debounce
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    // Verifica se o tempo mínimo de debounce passou (300ms)
    if (current_time - last_time > 300000)
    {
        last_time = current_time;

        //VERIFICA QUAL BOTÃO FOI PRESSIONADO
        if (gpio == button_0)
        {
            number--;
        }
        else if (gpio == button_1)
        {
            number++;
        }
        else if (gpio == button_2)
        {
            changeMatrixColor();
        }

        // GARANTE QUE OS NÚMEROS NÃO SAIAM DO INTERVALO DE 0 A 9
        if (number > 9)
        {
            number = 0;
        }
        else if (number < 0)
        {
            number = 9;
        }

        drawMatrix(number);
    }
}

// INICIALIZAÇÃO DOS BUTTONS
void initButtons(){
    gpio_init(button_0);
    gpio_set_dir(button_0, GPIO_IN);
    gpio_pull_up(button_0);

    gpio_init(button_1);
    gpio_set_dir(button_1, GPIO_IN);
    gpio_pull_up(button_1);

    gpio_init(button_2);
    gpio_set_dir(button_2, GPIO_IN);
    gpio_pull_up(button_2);
}

//INICIALIZAÇÃO DO LED VERMELHO
void initLed(){
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
}
