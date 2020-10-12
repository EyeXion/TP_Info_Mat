// A COMPLETER

/*
Service permettant de chornométrer jusqu'à 59mn 59s 99 1/100
Utilise un timer au choix (TIMER1 à TIMER4).
Utilise la lib MyTimers.h /.c
*/



#include "Chrono.h"
#include "MyTimer.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_utils.h"

#include "C:\Users\elies\Desktop\Cours\4A\Info Mat\Activit3USART\MDK-ARM\Usart.h"


// variable privée de type Time qui mémorise la durée mesurée
static Time Chrono_Time; // rem : static rend la visibilité de la variable Chrono_Time limitée à ce fichier 

// variable privée qui mémorise pour le module le timer utilisé par le module
static TIM_TypeDef * Chrono_Timer=TIM1; // init par défaut au cas où l'utilisateur ne lance pas Chrono_Conf avant toute autre fct.

// déclaration callback appelé toute les 10ms
void Chrono_Task_10ms(void);

static int etatPrecedentBouton = 0;

static int isChronoStarted = 1;

static void Chrono_Conf_io(){
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC); //On enable la clock pour le GPIOC
	
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_8,LL_GPIO_MODE_FLOATING); // On met la pin 8 en mode floating input
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_10,LL_GPIO_MODE_OUTPUT_2MHz); //On met la pin 10 en output
	LL_GPIO_SetPinOutputType(GPIOC,LL_GPIO_PIN_10,LL_GPIO_OUTPUT_OPENDRAIN); // On met la pin 10 en mode opendrain
	LL_GPIO_SetOutputPin(GPIOC,LL_GPIO_PIN_10);
}

void Chrono_Background(){
		//int etatButtonStartStop = LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_8);
	//int etatButtonReset = LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_13);
	int etatPresentBouton = LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_8);
	LL_mDelay(10);
	
	if (etatPresentBouton == 1 && etatPrecedentBouton == 0){
		etatPrecedentBouton = etatPresentBouton;
		if (isChronoStarted){
			isChronoStarted = 0;
			Chrono_Stop();
		}
		else{
			isChronoStarted = 1;
			Chrono_Start();
		}
	}
	else if (etatPresentBouton == 0 && etatPrecedentBouton == 1){
		etatPrecedentBouton = etatPresentBouton;
	}

	if (!LL_GPIO_IsInputPinSet(GPIOC,LL_GPIO_PIN_13)){ //Bouton en logique négative
		Chrono_Reset();
		isChronoStarted = 0;
		LL_GPIO_SetOutputPin(GPIOC,LL_GPIO_PIN_13);
	}
}

/**
	* @brief  Configure le chronomètre. 
  * @note   A lancer avant toute autre fonction.
	* @param  Timer : indique le timer à utiliser par le chronomètre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
	
void Chrono_Conf(TIM_TypeDef * Timer)
{
	//On l'appelle parceque c'est marqué dans le sujet (mode des PIN IO)
	Chrono_Conf_io();
	// Reset Time
	Chrono_Time.Hund=0;
	Chrono_Time.Sec=0;
	Chrono_Time.Min=0;
	
	
	// Fixation du Timer
	Chrono_Timer=Timer;

	// Réglage Timer pour un débordement à 10ms
	MyTimer_Conf(Chrono_Timer,999, 719);
	
	// Réglage interruption du Timer avec callback : Chrono_Task_10ms()
	MyTimer_IT_Conf(Chrono_Timer, Chrono_Task_10ms,3);
	
	// Validation IT
	MyTimer_IT_Enable(Chrono_Timer);
	
	Chrono_Reset();

}


/**
	* @brief  Démarre le chronomètre. 
  * @note   si la durée dépasse 59mn 59sec 99 Hund, elle est remise à zéro et repart
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Start(void)
{
	MyTimer_Start(Chrono_Timer);
}


/**
	* @brief  Arrête le chronomètre. 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Stop(void)
{
	MyTimer_Stop(Chrono_Timer);
}


/**
	* @brief  Remet le chronomètre à 0 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Reset(void)
{
  // Arrêt Chrono
	MyTimer_Stop(Chrono_Timer);

	// Reset Time
	Chrono_Time.Hund=0;
	Chrono_Time.Sec=0;
	Chrono_Time.Min=0;
	
	char msg[10];  //Contruction message à envoyer à l'USART
	msg[0] = Chrono_Time.Min/10  + 0x30 ;
	msg[1] = Chrono_Time.Min%10  + 0x30 ;
	msg[2] = ':';
	msg[3] = Chrono_Time.Sec/10  + 0x30 ;
	msg[4] = Chrono_Time.Sec%10  + 0x30 ;
	msg[5] = ':';
	msg[6] = Chrono_Time.Hund/10  + 0x30 ;
	msg[7] = Chrono_Time.Hund%10 + 0x30 ;
	msg[8] = ':';
	msg[9] = 0x0D;
	Usart_SendMsg(USART2,msg,sizeof(msg)/sizeof(char));
	
	isChronoStarted = 0;
}


/**
	* @brief  Renvoie l'adresse de la variable Time privée gérée dans le module Chrono.c
  * @note   
	* @param  Aucun
  * @retval adresse de la variable Time
  */
Time * Chrono_Read(void)
{
	return &Chrono_Time;
}




/**
	* @brief  incrémente la variable privée Chron_Time modulo 60mn 
  * @note   
	* @param  Aucun
  * @retval Aucun
  */
void Chrono_Task_10ms(void)
{ 
	Chrono_Time.Hund++;
	if (Chrono_Time.Hund==100)
	{
		Chrono_Time.Sec++;
		Chrono_Time.Hund=0;
	}
	if (Chrono_Time.Sec==60)
	{
		Chrono_Time.Min++;
		Chrono_Time.Sec=0;
	}
	if (Chrono_Time.Min==60)
	{
		Chrono_Time.Hund=0;
	}
	char msg[10];
	msg[0] = Chrono_Time.Min/10  + 0x30 ;
	msg[1] = Chrono_Time.Min%10  + 0x30 ;
	msg[2] = ':';
	msg[3] = Chrono_Time.Sec/10  + 0x30 ;
	msg[4] = Chrono_Time.Sec%10  + 0x30 ;
	msg[5] = ':';
	msg[6] = Chrono_Time.Hund/10  + 0x30 ;
	msg[7] = Chrono_Time.Hund%10 + 0x30 ;
	msg[8] = ':';
	msg[9] = 0x0D;
	Usart_SendMsg(USART2,msg,sizeof(msg)/sizeof(char));
}



