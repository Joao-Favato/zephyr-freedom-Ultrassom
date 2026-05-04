#include <zephyr.h>             // Funções básicas do Zephyr (ex: k_msleep)
#include <drivers/gpio.h>       // API para controle de GPIO
#include "pwm_z42.h"                // API para controle de PWM

#define TPM_IRQ_LINE TPM1_IRQn  // relaciona a interrupção ao timer TPM1
#define TPM_IRQ_PRIORITY 1      // define a prioridade da interrupção
// 50 ms convertido para ticks (~37500)
#define TPM_MODULE 37500

// 10 us → ~7 ticks (10 / 1.33)
#define PULSO_TICKS 7


volatile uint16_t captured= 0; 

void tpm1_isr(void *arg)
{
       TPM1->STATUS |= TPM_STATUS_CH0F_MASK; // zerra a flag que gerou a interrupção

       captured = TPM1->CONTROLS[0].CnV; // coloca o valor atual do timer na variável "captured"
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
    pwm_tpm_Init(TPM1, TPM_PLLFLL, 65535, TPM_CLK, PS_128, EDGE_PWM);
    pwm_tpm_Ch_Init(TPM1, 0, TPM_INPUT_CAPTURE_RISING| TPM_CHANNEL_INTERRUPT, GPIOA, 12);

    while (1)
    {
        printk("Valor do TPM1: %u\n", captured);
        k_msleep(1000); 
    }
}