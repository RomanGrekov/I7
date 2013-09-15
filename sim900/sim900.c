#include "sim900.h"

void InitSim900(void){
    RCC->APB2ENR |= CLOCK_REG; //Clock port

    MANAGE_PORT->SET_PWR_REG &= ~(PWR_CNF_REG); //Init PWR_KEY
    MANAGE_PORT->SET_PWR_REG |= (PWR_MODE_REG0);// max freq 10 MHz
    MANAGE_PORT->SET_PWR_REG &= ~(PWR_MODE_REG1);//

    MANAGE_PORT->SET_STATUS_REG |= STATUS_CNF_REG0;// Init STATUS. Input
    MANAGE_PORT->SET_STATUS_REG &= ~STATUS_CNF_REG1;// Init STATUS. Input
    MANAGE_PORT->SET_STATUS_REG &= ~(STATUS_MODE_REG0 | STATUS_MODE_REG1);


}

uint8_t SwitchSim900(uint8_t state, uint8_t timeout){
	uint8_t count=0;
	if (state){
		while ((MANAGE_PORT->CHECK_REG & STATUS_STATE) == 0){ //while status != 1
			if (count > timeout)return 1;
			MANAGE_PORT->SET_REG |= PWR_KEY; //turn on pwr key
			delay_timer_ms(1000);
			MANAGE_PORT->SET_REG &= ~PWR_KEY; //turn off pwr key
			delay_timer_ms(2000);
			count++;
		}
	}
	else{
		while ((MANAGE_PORT->CHECK_REG & STATUS_STATE) != 0){ //while status != 0
			if (count > timeout)return 1;
			MANAGE_PORT->SET_REG |= PWR_KEY; //turn on pwr key
			delay_timer_ms(1000);
			MANAGE_PORT->SET_REG &= ~PWR_KEY; //turn off pwr key
			delay_timer_ms(2000);
			count++;
		}
	}
	return 0;
}
