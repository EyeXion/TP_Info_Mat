// TOUT A FAIRE !! //

/*
 indispensable pour pouvoir adresser les registres des périphériques.
 Rem : OBLIGATION d'utiliser les définitions utiles contenues dans ce fichier (ex : TIM_CR1_CEN, RCC_APB1ENR_TIM2EN ...)
 pour une meilleure lisibilité du code.

 Pour les masques, utiliser également les définitions proposée
 Rappel : pour mettre à 1  , reg = reg | Mask (ou Mask est le représente le ou les bits à positionner à 1)
				  pour mettre à 0  , reg = reg&~ Mask (ou Mask est le représente le ou les bits à positionner à 0)
 
*/ 
#include "stm32f103xb.h" 

void MyTimer_Conf(TIM_TypeDef * Timer,int Arr, int Psc){
	
	//Activation horloge selon timer utilisé
	if (Timer == TIM1){
		RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_TIM1EN;
	}
	if (Timer == TIM2){
			RCC->APB1ENR = RCC->APB1ENR | RCC_APB1ENR_TIM2EN;
	}
	else if (Timer == TIM3){
			RCC->APB1ENR = RCC->APB1ENR | RCC_APB1ENR_TIM3EN;
	}
	else{
			RCC->APB1ENR = RCC->APB1ENR | RCC_APB1ENR_TIM4EN;
	}		
	
	Timer->ARR = Arr;
	Timer->PSC = Psc;
	
}

void MyTimer_Start(TIM_TypeDef * Timer){
	//Mise à 1 du bit CEN de CR1 = activation timer
	Timer->CR1 |= 0x1;
}

void MyTimer_Stop(TIM_TypeDef * Timer){
	//Mise à 0 du bit CEN de CR1 =  stop timer
	Timer->CR1 &= ~0x1;
}

