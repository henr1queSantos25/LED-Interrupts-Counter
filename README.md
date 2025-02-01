# 🔢 Tarefa 1: Controle de LED 5x5 com Raspberry Pi Pico W  

Este repositório contém um exemplo de código para a implementação de **uma matriz de LEDs 5x5**, utilizando **botões físicos para alterar os números exibidos**.  

## 🎯 Objetivo  

- Demonstrar como controlar uma **matriz de LEDs 5x5** usando o Raspberry Pi Pico W.  
- Implementar **interrupções de GPIO** para capturar eventos dos botões físicos.  
- Utilizar **debounce** para evitar múltiplas detecções de um único toque.  
- Integrar **código PIO** para otimizar a exibição na matriz.  

## 📹 Vídeo do Projeto  

- **[Ver Vídeo](#)**   

## 🛠️ Tecnologias Utilizadas  

- **Linguagem de Programação**: C.  
- **Placa Microcontroladora**: Raspberry Pi Pico W.  
- **Bibliotecas**:  
  - Pico SDK.  
  - PIO (para controle da matriz de LEDs).  
- **Ferramentas**:  
  - Wokwi (Simulação).  
  - VS Code (Ambiente de Desenvolvimento).  

## 🗂️ Estrutura do Repositório  

- `counterLed.c`: Código-fonte principal contendo a lógica de controle da matriz de LEDs.  
- `functions.c`: Implementação de funções auxiliares para exibição e alteração de cor.  
- `counterLed.pio`: Código PIO para controle eficiente dos LEDs.  
- `README.md`: Documentação detalhada do projeto.  
- `diagram.json`: Configuração do circuito para simulação no Wokwi.  

## 🚀 Como Funciona  

O sistema exibe um **número de 0 a 9 na matriz de LEDs 5x5**, e os botões físicos permitem **modificar a exibição**:  

1. **Botão 0 (GP5):** **Diminui** o número exibido.  
2. **Botão 1 (GP6):** **Aumenta** o número exibido.  
3. **Botão 2 (GP22):** **Altera a cor dos LEDs.**  

✅ **A matriz 5x5 é controlada via PIO** para melhor desempenho.  
✅ **Debounce** é aplicado para evitar leituras erradas dos botões.  
✅ **LED Vermelho (GP13)** pisca para indicar funcionamento.  

## 🔌 Configuração do Hardware  

| Componente        | Pino do Pico W |  
|------------------|---------------|  
| Matriz de LEDs 5x5 | GP7          |  
| Botão 0 (Diminuir número) | GP5  |  
| Botão 1 (Aumentar número) | GP6  |  
| Botão 2 (Mudar cor) | GP22  |  
| LED Vermelho | GP13  |  

## 📜 Código Simplificado  

```c
// Interrupção do botão  
void gpio_irq_handler(uint gpio, uint32_t events) {  
    uint32_t current_time = to_us_since_boot(get_absolute_time());  

    if (current_time - last_time > 300000) {  
        last_time = current_time;  

        if (gpio == button_0) number--;  
        else if (gpio == button_1) number++;  
        else if (gpio == button_2) changeMatrixColor();  

        if (number > 9) number = 0;  
        else if (number < 0) number = 9;  

        drawMatrix(number);  
    }  
}  
