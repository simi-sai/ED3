/* Ejercicio 6 - Guia 2024 */
/* Realizar un programa que configure el puerto P2.0 y P2.1 para que provoquen
una interrupción por flanco de subida para el primer pin y por flanco de bajada
para el segundo. Cuando la interrupción sea por P2.0 se enviará por el pin P0.0 la
secuencia de bits 010011010. Si la interrupción es por P2.1 se enviará por el pin
P0.1 la secuencia 011100110. Las secuencias se envían únicamente cuando se
produce una interrupción, en caso contrario la salida de los pines tienen valores 1
lógicos. */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif
#include <cr_section_macros.h>

void config_ports(void);
void config_eint(void);
void delay(uint32_t);

int main(void) {
	config_ports();
	config_eint();

	while(1) {
		LPC_GPIO0->FIOSET |= (3 << 0);
	}
    return 0 ;
}

void config_ports(void) {
	// Por default los pines son GPIO
	LPC_GPIO0->FIODIR |= 0x3;    // P0.0 - P0.1 salidas
	LPC_GPIO2->FIODIR &= ~(0x3); // P2.0 - P2.1 entradas
	LPC_GPIO0->FIOMASK &= ~(0x3);

	// 11 -> P2.0 & 00 -> P2.1
	LPC_PINCON->PINMODE4 &= ~(0b1100);
}

void config_eint(void) {
	LPC_GPIOINT->IO2IntEnR |= (1 << 0);
	LPC_GPIOINT->IO2IntEnF |= (1 << 1);

	NVIC_EnableIRQ(EINT3_IRQn);
}

void EINT3_IRQHandler(void) {
		// 010011010
	if(LPC_GPIOINT->IO2IntStatR & (1 << 0)) {
		uint8_t secuencia[9] = {0, 1, 0, 0, 1, 1, 0, 1, 0};
		uint8_t num;
		for(num = 0; num < 10; num++) {
			if(secuencia[num] == 0) {
				LPC_GPIO0->FIOCLR |= (1 << 0);
			} else {
				LPC_GPIO0->FIOSET |= (1 << 0);
			}
			delay(100);
		}
		LPC_GPIOINT->IO2IntClr |= (1 << 0);
	} else if (LPC_GPIOINT->IO2IntStatR & (1 << 1)){
		// 011100110
		uint8_t secuencia[9] = {0, 1, 1, 1, 0, 0, 1, 1, 0};
		uint8_t num;

		for(num = 0; num < 10; num++) {
			if(secuencia[num] == 0) {
				LPC_GPIO0->FIOCLR |= (1 << 1);
			} else {
				LPC_GPIO0->FIOSET |= (1 << 1);
			}
			delay(100);
		}
		LPC_GPIOINT->IO2IntClr |= (1 << 1);
	}
}

void delay(uint32_t tiempo) {
	uint32_t contador;

	for(contador = 0; contador < tiempo; contador++){}
}
