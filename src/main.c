#include <pwm_z42.h>

// 50 ms convertido para ticks (~37500)
#define TPM_MODULE 37500

// 10 us → ~7 ticks (10 / 1.33)
#define PULSO_TICKS 7

int main(void)
{
    pwm_tpm_Init(TPM1,
                 TPM_PLLFLL,
                 TPM_MODULE,  
                 TPM_CLK,
                 PS_64,
                 EDGE_PWM);

    // TPM1_CH1 → PORTA13 (válido)
    pwm_tpm_Ch_Init(TPM1, 1, TPM_PWM_H, GPIOA, 13);

    // Define duty (pulso de 10 us)
    pwm_tpm_CnV(TPM1, 1, PULSO_TICKS);

    while (1)
    {
    }
}