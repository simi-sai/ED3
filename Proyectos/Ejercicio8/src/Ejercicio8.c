/* Ejercicio 8 - Guia 2024 */
/* Configurar la interrupción externa EINT1 para que interrumpa por flanco de
bajada y la interrupción EINT2 para que interrumpa por flanco de subida. En la
interrupción por flanco de bajada configurar el systick para desbordar cada 25
mseg, mientras que en la interrupción por flanco de subida configurarlo para que
desborde cada 60 mseg. Considerar que EINT1 tiene mayor prioridad que EINT2.
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

void config_exti(void);
void config_pin(void);
void config_systick(void);

int main(void) {
	config_pin();
	config_systick();
	config_exti();

	while(1){
		//do nothing
	}

    return 0;
}

void config_pin(void) {
	LPC_PINCON->PINSEL4 |= (1 << 22);  //Funcion 01 - EINT1
	LPC_PINCON->PINSEL4 |= (1 << 24);  //Funcion 01 - EINT2
}

void config_exti(void) {
	LPC_SC->EXTMODE |= (3 << 1);  // ~(3<<0)

	LPC_SC->EXTPOLAR |= (1 << 2);
	LPC_SC->EXTPOLAR &= ~(3 << 0);

	LPC_SC->EXTINT |= (3 >> 1);

	NVIC_EnableIRQ(EINT1_IRQn);
	NVIC_EnableIRQ(EINT2_IRQn);
}

void config_systick(void) {
	SysTick->LOAD = 999999;
	SysTick->VAL = 0;
	SysTick->CTRL = 0b101; //CPU Clock; Habilita el Systick; desaibilita IRQ.
	//NVIC_EnableIRQ(SysTick_IRQn);
}

void EINT1_IRQHandler(void) {
	// LOAD = 25 ms * 100 MHz - 1 = 2.499.999
	SysTick->LOAD = 2499999;
	SysTick->VAL = 0;
	LPC_SC->EXTINT |= (1 << 1);
}

void EINT2_IRQHandler(void) {
	// LOAD = 60 ms * 100 MHz - 1 = 5.999.999
	SysTick->LOAD = 5999999;
	SysTick->VAL = 0;
	LPC_SC->EXTINT |= (1 << 2);
}
