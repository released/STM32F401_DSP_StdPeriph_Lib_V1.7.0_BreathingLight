/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

/* Platform config -----------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "Macro.h"
#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
 

/* Define config -------------------------------------------------------------*/
#define LED2_TOGGLE(void)	(GPIO_ToggleBits(GPIOA,GPIO_Pin_5))

#define USE_GPIO
//#define USE_PWM

/* Macro ---------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
extern __IO uint8_t UartRxBuffer;
extern __IO uint8_t UartRxFlag;

void BreathingLight_Handler(void);
void BreathingLight_Config(void);

void PollingProcedure(void);

void LED_Config(void);

void TIM4_Config(void);
void TIM3_Config(void);

void USART_Test(void);
void USART_Config(void);

void SysTickConfig(void);
void TimingDelay_Decrement(void);
void Delay(__IO uint32_t uTime);
/* Exported constants --------------------------------------------------------*/

#endif  /* __HW_CONFIG_H */

