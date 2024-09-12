/* Ejercicio 3 - Guia 2024 */
/* Configurar el pin P0.4 como entrada digital con resistencia de pull down y
utilizarlo para decidir si el valor representado por los pines P0.0 al P0.3 van a ser
sumados o restados al valor guardado en la variable "acumulador". El valor inicial
de "acumulador" es 0. */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif
#include <cr_section_macros.h>

void config_ports(void);
uint8_t fetch(void);
void delay(uint32_t);

int main(void) {
	config_ports();

	volatile uint8_t aux;
	static uint32_t acumulador = 0;

	while(1) {
		aux = fetch();

		if(aux == 0) {
			acumulador += (LPC_GPIO0->FIOPIN & 0xF);
		} else {
			acumulador -= (LPC_GPIO0->FIOPIN & 0xF);
		}

		delay(1000);
	}
    return 0 ;
}

void config_ports(void) {
	LPC_PINCON->PINSEL0 |= ~(0b11111); // GPIO P0.0 a P0.4
	LPC_PINCON->PINMODE0 |= (3 << 8);  // PULL-DOWN P0.4

	LPC_GPIO0->FIODIR &= ~(0x1F);      // P0.0 a P0.4 entradas
	LPC_GPIO0->FIOMASK &= ~(0x1F);     // Mask al resto de pines
}

/* Lee el valor del pin P0.4 */
uint8_t fetch(void) {
	uint8_t temp = LPC_GPIO0->FIOPIN & (1 << 5);

	return temp;
}
