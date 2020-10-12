
#include "Usart.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_usart.h"


void Usart_Config(USART_TypeDef *USARTx, int baudRate){
	if (USARTx == USART1){
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1); //On enable la clock pour l'USARt	
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA); //enable la clock du gpio où est l'USART
		LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_9,LL_GPIO_MODE_ALTERNATE); // Mode fonction alternative PIN USART Tx
	}
	else if (USARTx == USART2){
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2); //On enable la clock pour l'USART	
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
		LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_2,LL_GPIO_MODE_ALTERNATE);
	}
	else if (USARTx == USART3){
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3); //On enable la clock pour l'USART	
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);		
		LL_GPIO_SetPinMode(GPIOB,LL_GPIO_PIN_10,LL_GPIO_MODE_ALTERNATE);
	}
	LL_USART_Enable(USARTx); //Enable Usart
	LL_USART_EnableDirectionTx(USARTx); //Enable direction Tx
	LL_USART_SetParity(USARTx,LL_USART_PARITY_NONE); // disable parity bit
	LL_USART_SetStopBitsLength(USARTx,LL_USART_STOPBITS_1);
	USARTx->BRR = baudRate; //Set Baud Rate /* ATTENTION, REFAIRE BAUD RATE) REGARDER GOOGLE
}

void Usart_SendMsg(USART_TypeDef *USARTx, char * msg, int tailleMsg){
	int index  = 0;
	while(index < tailleMsg){
		if (LL_USART_IsActiveFlag_TXE(USARTx)){ //On regarde si le flag de transmission terminée est actif
			LL_USART_TransmitData8(USARTx, (uint8_t) msg[index]); //On envoie le message (8 bits)
			index++;
		}
	}
}


