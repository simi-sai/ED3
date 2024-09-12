/* Ejercicio 1 - Guia 2024 */
/* Una famosa empresa de calzados a incorporado a sus zapatillas 10 luces leds
comandadas por un microcontrolador LPC1769 y ha pedido a su grupo de
ingenieros que diseñen 2 secuencias de luces que cada cierto tiempo se vayan
intercalando (secuencia A - secuencia B- secuencia A- ... ). Como todavía no se
ha definido la frecuencia a la cual va a funcionar el CPU del microcontrolador, las
funciones de retardos que se incorporen deben tener como parametros de
entrada variables que permitan modificar el tiempo de retardo que se vaya a
utilizar finalmente. Se pide escribir el código que resuelva este pedido,
considerando que los leds se encuentran conectados en los puertos P0,0 al P0.9 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <stdio.h>
#include <stdlib.h>

void config_ports(void);
void secuencia(uint8_t);
void delay(uint32_t);
void clear();

int main(void) {
	uint32_t time = 100;
	config_ports();
	LPC_GPIO0->FIOSET = 0b0101010101;

	while(1) {
		secuencia();
		delay(time);
	}

    return 0;
}

void config_ports(void) {
	LPC_PINCON->PINSEL0 &= ~(0xFFFFF); // P0.0 a P0.9 GPIO
	LPC_GPIO0->FIODIR |= 0x3FF; 	  // Configura P0.0 a P0.9 como salidas
	LPC_GPIO0->FIOMASK &= ~(0x3FF);   // Habilitar P0.0 a P0.9
}

/* Prender LEDs impares */
void secuencia(void){
	LPC_GPIO0->FIOCLR = 0x3FF;
	uint32_t temp = LPC_GPIO0->FIOPIN;

	LPC_GPIO0->FIOSET |= ~temp;
}

void delay(uint32_t tiempo) {
	volatile uint32_t counter;
	for(counter = 0; counter < tiempo; counter ++);
}
