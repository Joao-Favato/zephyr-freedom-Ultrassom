#include <zephyr.h>
#include "ultrassom.h"

void main(void)
{
    ultrassom_init();
    printk("--- Biblioteca de Ultrassom Inicializada ---\n");

    while (1) {
        float d = ultrassom_get_distancia();
        printk("Distancia: %.2f cm\n", (double)d);
        k_msleep(1000);
    }
}