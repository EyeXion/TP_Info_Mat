// TOUT A FAIRE !! //

/*
 indispensable pour pouvoir adresser les registres des p�riph�riques.
 Rem : OBLIGATION d'utiliser les d�finitions utiles contenues dans ce fichier (ex : TIM_CR1_CEN, RCC_APB1ENR_TIM2EN ...)
 pour une meilleure lisibilit� du code.

 Pour les masques, utiliser �galement les d�finitions propos�e
 Rappel : pour mettre � 1  , reg = reg | Mask (ou Mask est le repr�sente le ou les bits � positionner � 1)
				  pour mettre � 0  , reg = reg&~ Mask (ou Mask est le repr�sente le ou les bits � positionner � 0)
 
*/ 
#include "stm32f103xb.h" 


void (* pTIM1_Overflow) (void); //Pointeurs de fonctions que l'on appelle dans le handler et d�finis dans IT_Conf
void (* pTIM2_Overflow) (void);
void (* pTIM3_Overflow) (void);
void (* pTIM4_Overflow) (void);


void MyTimer_Conf(TIM_TypeDef * Timer,int Arr, int Psc){
	
	//Activation horloge selon timer utilis�
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
	//Mise � 1 du bit CEN de CR1 = activation timer
	Timer->CR1 |= 0x1;
}

void MyTimer_Stop(TIM_TypeDef * Timer){
	//Mise � 0 du bit CEN de CR1 =  stop timer
	Timer->CR1 &= ~0x1;
}

void MyTimer_IT_Conf(TIM_TypeDef * Timer, void (*IT_function) (void),int Prio){
	Timer->DIER |= 0x1; //Enable Update IT (d�bordement du timer)
	if (Timer == TIM1){
		NVIC->IP[TIM1_UP_IRQn] = Prio<<4;
		NVIC->ISER[0] |= 0x1<<TIM1_UP_IRQn;
		pTIM1_Overflow = IT_function;
	}
	if (Timer == TIM2){
		NVIC->IP[TIM2_IRQn] = Prio<<4;
		NVIC->ISER[0] |= 0x1<<TIM2_IRQn;
		pTIM2_Overflow = IT_function;
	}
	if (Timer == TIM3){
		NVIC->IP[TIM3_IRQn] = Prio<<4;
		NVIC->ISER[0] |= 0x1<<TIM3_IRQn;
		pTIM3_Overflow = IT_function;
	}
	if (Timer == TIM4){
		NVIC->IP[TIM4_IRQn] = Prio<<4;
		NVIC->ISER[0] |= 0x1<<TIM4_IRQn;
		pTIM4_Overflow = IT_function;
	}
}


void MyTimer_IT_Disable(TIM_TypeDef * Timer){
	Timer->DIER |= 0x0; //Disable Update IT (d�bordement du timer)
	if (Timer == TIM1){
		NVIC->ISER[0] |= 0x0<<TIM1_UP_IRQn;
	}
	if (Timer == TIM2){
		NVIC->ISER[0] |= 0x0<<TIM2_IRQn;
	}
	if (Timer == TIM3){
		NVIC->ISER[0] |= 0x0<<TIM3_IRQn;
	}
	if (Timer == TIM4){
		NVIC->ISER[0] |= 0x0<<TIM4_IRQn;
	}
}

void TIM1_UP_IRQHandler(void){  //Fonctions qui sont les handler appel�e par le NVIC lors des interruptions
	(*pTIM1_Overflow)(); //Contien le faux "handler" qui lui contient des instructions
	TIM1->SR = 0x0;
}
void TIM2_IRQHandler(void){
	(*pTIM2_Overflow)();
	TIM2->SR = 0x0;
}
void TIM3_IRQHandler(void){
	(*pTIM3_Overflow)();
	TIM3->SR = 0x0;
}
void TIM4_IRQHandler(void){
	(*pTIM4_Overflow)();
	TIM4->SR = 0x0;
}
