#include <zephyr.h>             // Funções básicas do Zephyr (ex: k_msleep)
#include <drivers/gpio.h>       // API para controle de GPIO
#include "pwm_z42.h"                // API para controle de PWM

#define TPM_IRQ_LINE TPM1_IRQn  // relaciona a interrupção ao timer TPM1
#define TPM_IRQ_PRIORITY 1      // define a prioridade da interrupção
// 50 ms convertido para ticks (~37500)
#define TPM_MODULE 37500

// 1 tick = 0,020618 microssegundos (com base na frequência de 48.5 MHz)
#define PULSO_TICKS 485

// Variáveis para o cálculo do intervalo
volatile uint16_t t_atual = 0;
volatile uint16_t t_anterior = 0;
volatile uint16_t intervalo_ticks = 0;
volatile uint32_t total_pulsos = 0;

volatile uint16_t captured= 0; 

void tpm1_isr(void *arg)
{
       // 1. Limpa a flag de interrupção
    TPM1->STATUS |= TPM_STATUS_CH0F_MASK; 

    // 2. Armazena o tempo do pulso anterior
    t_anterior = t_atual;

    // 3. Captura o novo "carimbo de tempo" do hardware
    t_atual = TPM1->CONTROLS[0].CnV;

    // 4. Calcula a diferença (Intervalo em ticks)
    // Se t_atual for menor que t_anterior, a aritmética de 16 bits
    // resolve o overflow automaticamente (ex: 5 - 65530 = 11 ticks)
    intervalo_ticks = t_atual - t_anterior;

    // Contador apenas para controle de debug
    total_pulsos++;
}

void main(void)
{
    // Conecta a interrupção via Zephyr
    IRQ_CONNECT(TPM_IRQ_LINE, TPM_IRQ_PRIORITY, tpm1_isr, NULL, 0);
    irq_enable(TPM_IRQ_LINE);

    // Geração do trigger na porta D, pino 3 (TPM0_CH3)
    pwm_tpm_Init(TPM0, TPM_PLLFLL, TPM_MODULE, TPM_CLK, PS_1, EDGE_PWM);
    pwm_tpm_Ch_Init(TPM0, 3, TPM_PWM_H, GPIOD, 3);
    pwm_tpm_CnV(TPM0, 3, PULSO_TICKS);

    // Inicializa TPM1 com módulo e prescaler desejado para Input Capture
    pwm_tpm_Init(TPM1, TPM_PLLFLL, 65535, TPM_CLK, PS_1, EDGE_PWM);
    pwm_tpm_Ch_Init(TPM1, 0, TPM_INPUT_CAPTURE_BOTH| TPM_CHANNEL_INTERRUPT, GPIOA, 12);

    printk("--- Iniciando Medicao de Intervalo ---\n");

    while (1)
    {
        // Usamos a frequência que acabamos de descobrir (48.5 MHz)
    // Para microssegundos: (ticks * 1.000.000) / frequencia
    float largura_us = (intervalo_ticks * 1000000.0) / 48500000.0;

    printk("Largura: %u ticks | Tempo Real: %.2f us \n", 
            intervalo_ticks, (double)largura_us);

    k_msleep(1000);

        
    }
}