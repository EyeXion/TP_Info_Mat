
#ifndef USART_H
#define USART_H


#include "stm32f103xb.h" 

//Config de l'USART
void Usart_Config(USART_TypeDef *USARTx, int baudRate);

//Fonction qui ecrit toute les 10ms le msg dans DR
void Usart_SendMsg(USART_TypeDef *USARTx, char * msg, int tailleMsg);





#endif
