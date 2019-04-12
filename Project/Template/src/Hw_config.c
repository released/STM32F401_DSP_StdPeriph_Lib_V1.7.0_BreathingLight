/* Includes ------------------------------------------------------------------*/
#include "Hw_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
  #define GETCHAR_PROTOTYPE int __io_getchar(void)  
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
  #define GETCHAR_PROTOTYPE int fgetc(FILE *f)  
#endif /* __GNUC__ */

// TODO: for printf function , need to confirm use USART2 or USART2
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART2, (uint8_t) ch);

  /* Loop until transmit data register is empty */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t uwTimingDelay;

/*uart variable*/
__IO uint8_t UartRxBuffer = 0;
__IO uint8_t UartRxFlag = FALSE;

/*PWM variable*/
uint16_t TimerPeriod = 0;
#define	MixedLIghtLED2_R(duty)		TIM_SetCompare2(TIM1,((uint16_t) (((uint32_t) duty * (TimerPeriod - 1)) / 100)))
#define	MixedLIghtLED2_G(duty)		TIM_SetCompare3(TIM1,((uint16_t) (((uint32_t) duty * (TimerPeriod - 1)) / 100)))
#define	MixedLIghtLED2_B(duty)		TIM_SetCompare1(TIM1,((uint16_t) (((uint32_t) duty * (TimerPeriod - 1)) / 100)))

#define	MixedLIghtLED1_R(duty)		TIM_SetCompare2(TIM3,((uint16_t) (((uint32_t) duty * (TimerPeriod - 1)) / 100)))
#define	MixedLIghtLED1_G(duty)		TIM_SetCompare1(TIM3,((uint16_t) (((uint32_t) duty * (TimerPeriod - 1)) / 100)))
#define	MixedLIghtLED1_B(duty)		TIM_SetCompare3(TIM3,((uint16_t) (((uint32_t) duty * (TimerPeriod - 1)) / 100)))

/* Private functions ---------------------------------------------------------*/

void LED1_Blue_OFF(void)
{
	 GPIO_ResetBits(GPIOB,GPIO_Pin_0);
//	 GPIO_ResetBits(GPIOB,GPIO_Pin_4);
//	 GPIO_ResetBits(GPIOB,GPIO_Pin_5);	
}

void LED1_Blue_ON(void)
{
	 GPIO_SetBits(GPIOB,GPIO_Pin_0);
//	 GPIO_SetBits(GPIOB,GPIO_Pin_4);
//	 GPIO_SetBits(GPIOB,GPIO_Pin_5);	
}

void LED1_White_OFF(void)
{
	 GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	 GPIO_ResetBits(GPIOB,GPIO_Pin_4);
	 GPIO_ResetBits(GPIOB,GPIO_Pin_5);	
}

void LED1_White_ON(void)
{
	 GPIO_SetBits(GPIOB,GPIO_Pin_0);
	 GPIO_SetBits(GPIOB,GPIO_Pin_4);
	 GPIO_SetBits(GPIOB,GPIO_Pin_5);	
}

void LED2_Green_OFF(void)
{
//	 GPIO_ResetBits(GPIOA,GPIO_Pin_8);
//	 GPIO_ResetBits(GPIOA,GPIO_Pin_9);
	 GPIO_ResetBits(GPIOA,GPIO_Pin_10);	
}

void LED2_Green_ON(void)
{
//	 GPIO_SetBits(GPIOA,GPIO_Pin_8);
//	 GPIO_SetBits(GPIOA,GPIO_Pin_9);
	 GPIO_SetBits(GPIOA,GPIO_Pin_10);	
}

void LED2_PWMConfig(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	uint16_t Channel1Pulse = 0;
	uint16_t Channel2Pulse = 0;	
	uint16_t Channel3Pulse = 0;
	
//	RCC_PCLK1Config(RCC_HCLK_Div2);
	
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1);
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);	

	/*TIMx Frequency =  TIMxCLK / ((TIM_Period + 1) * (TIM_Prescaler + 1))	
		=> example : 
			Freq = 20000 Hz 	= TIMxCLK / ((TIM_Period + 1) * (TIM_Prescaler + 1))
					  			= 84000000 / (200-1     + 1) * (21 -1        + 1))
	*/
	TimerPeriod = (SystemCoreClock / 20000) - 1;	// 20k
	/* Compute CCR1 value to generate a duty cycle at 50% for channel 1 */
	Channel1Pulse = (uint16_t) (((uint32_t) 0 * (TimerPeriod - 1)) / 100);
	Channel2Pulse = (uint16_t) (((uint32_t) 0 * (TimerPeriod - 1)) / 100);	
	Channel3Pulse = (uint16_t) (((uint32_t) 0 * (TimerPeriod - 1)) / 100);

	/* TIMx base configuration */
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);	
	TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
	TIM_TimeBaseStructure.TIM_Prescaler = (1 -1);
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;	
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;		
	TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_Pulse = Channel3Pulse;
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);

	/* TIMx enable counter */
	TIM_Cmd(TIM1, ENABLE);

	/* TIM1 Main Output Enable */
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void LED1_PWMConfig(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	uint16_t Channel1Pulse = 0;
	uint16_t Channel2Pulse = 0;	
	uint16_t Channel3Pulse = 0;
	
//	RCC_PCLK1Config(RCC_HCLK_Div2);
	
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

  	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
  	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);
  	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3);	

	/*TIMx Frequency =  TIMxCLK / ((TIM_Period + 1) * (TIM_Prescaler + 1))	
		=> example : 
			Freq = 20000 Hz 	= TIMxCLK / ((TIM_Period + 1) * (TIM_Prescaler + 1))
					  			= 84000000 / (200-1     + 1) * (21 -1        + 1))
	*/
	TimerPeriod = (SystemCoreClock / 20000) - 1;	// 20k
	/* Compute CCR1 value to generate a duty cycle at 50% for channel 1 */
	Channel1Pulse = (uint16_t) (((uint32_t) 0 * (TimerPeriod - 1)) / 100);
	Channel2Pulse = (uint16_t) (((uint32_t) 0 * (TimerPeriod - 1)) / 100);	
	Channel3Pulse = (uint16_t) (((uint32_t) 0 * (TimerPeriod - 1)) / 100);

	/* TIMx base configuration */
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);	
	TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
	TIM_TimeBaseStructure.TIM_Prescaler = (1 -1);
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;		
	TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);

	TIM_OCInitStructure.TIM_Pulse = Channel3Pulse;
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);

	/* TIMx enable counter */
	TIM_Cmd(TIM3, ENABLE);

	/* TIM1 Main Output Enable */
	TIM_CtrlPWMOutputs(TIM3, ENABLE);
}


void BreathingLight_Handler(void)
{
	#if defined (USE_GPIO)		
	#define INC_VALUE   		5
	#define PWM_WIDTH    	1500

    static uint8_t state=0;   
	static uint16_t led_i = 0 ;
	static uint16_t led_H = 0 ;
	static uint16_t led_W = 0 ;

    ++led_i;
     if(led_i < led_H)
     {
//		LED1_White_ON();
		LED1_Blue_ON();
//		LED2_Green_ON();
     }
     else if((led_i >= led_H) && (led_i < led_W))
     {
//		LED1_White_OFF(); 
		LED1_Blue_OFF();
//		LED2_Green_OFF();
     }
     else
     {
         led_i = 0;
         led_W = PWM_WIDTH;
         if(state==0)
         {
             led_H += INC_VALUE;
             if(led_H >= PWM_WIDTH)
             {
                 state = 1;
             }
         }
         else
         {            
             led_H -= INC_VALUE;
             if(led_H <= 100)
             {
                 state = 0;
             }
         }
     }
	 #endif

	#if defined (USE_PWM)
	static uint16_t led_i = 0;
	static uint8_t duty = 0;
	static uint8_t flag = 1;
	
	#define CYCLE    	(320)
	
	if (led_i++ >=CYCLE)
	{
		led_i = 0;

		if (flag)
		{
			duty++;
		}
		else
		{
			duty--;
		}

		if (duty==100)
		{
			flag = 0;
//			printf("%s:%2d\r\n",__FUNCTION__,flag);
		}

		if (duty==5)
		{
			flag = 1;
//			printf("%s:%2d\r\n",__FUNCTION__,flag);			
		}
		
//		MixedLIghtLED1_R(duty);
		MixedLIghtLED1_G(duty);
//		MixedLIghtLED1_B(duty);

//		MixedLIghtLED2_R(duty);
//		MixedLIghtLED2_G(duty);
//		MixedLIghtLED2_B(duty);
		
	}

	#endif
	 
}

/*
	B : TIM1_CH1/PA8
	R :	TIM2_CH4/PA2
	G :	TIM2_CH3/PA3
*/

/*
	B : TIM3_CH3/PB0
	R :	TIM3_CH2/PB5
	G :	TIM3_CH1/PB4
*/

void BreathingLight_Config(void)
{
	#if defined (USE_GPIO)	
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clock */
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	#endif

	#if defined (USE_PWM)
	LED1_PWMConfig();
//	LED2_PWMConfig();

	#endif

}

void PollingProcedure(void)
{	
	USART_Test();

}
void LED_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

/*
	TIMxCLK = PCLK1 = HCLK = SystemCoreClock
	TIMx counter clock = TIMxCLK /((Prescaler + 1)*(Period + 1))
	                = 84 MHz / ((11+1)*(6999+1))
	                = 1000 Hz 
     ==> TIMx counter period = 1 ms
*/
void TIM4_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	/* PCLK1 = HCLK/4 */
	RCC_PCLK1Config(RCC_HCLK_Div2);//TIM3CLK = (HCLK/4)x2 = (180 MHz/4)x2 = 90 MHz 
	
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	/* TIMx base configuration */
	TIM_TimeBaseStructure.TIM_Period = (7000 -1);
	TIM_TimeBaseStructure.TIM_Prescaler = (12 -1);
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	/* TIMx Interrupts enable */
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

	/* TIMx enable counter */
	TIM_Cmd(TIM4, ENABLE);

	/* Enable the TIMx gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

}

/*
	TIMxCLK = PCLK1 = HCLK = SystemCoreClock
	TIMx counter clock = TIMxCLK /((Prescaler + 1)*(Period + 1))
	                = 84 MHz / ((11+1)*(6999+1))
	                = 1000 Hz 
     ==> TIMx counter period = 5 us
*/
void TIM3_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	/* PCLK1 = HCLK/4 */
	RCC_PCLK1Config(RCC_HCLK_Div2);//TIM3CLK = (HCLK/4)x2 = (180 MHz/4)x2 = 90 MHz 
	
	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* TIMx base configuration */
	TIM_TimeBaseStructure.TIM_Period = (SystemCoreClock / 200000 ) -1;
	TIM_TimeBaseStructure.TIM_Prescaler = (1 -1);
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	/* TIMx Interrupts enable */
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	/* TIMx enable counter */
	TIM_Cmd(TIM3, ENABLE);

	/* Enable the TIMx gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

}

void USART_Test(void)
{
	
	if (UartRxFlag)
	{
		switch (UartRxBuffer)
		{
			case '1':
			
				break;	
				
			case 'Z':
			case 'z':				
				NVIC_SystemReset();
				
				break;				
			default : 
				UartRxFlag = FALSE;				
				break;
		}
		UartRxFlag = FALSE;
	}
}

void USART_Config(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* Enable UART clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* Connect PXx to USARTx_Tx*/
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);

	/* Connect PXx to USARTx_Rx*/
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);

	/* Configure USART Tx as alternate function  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART Rx as alternate function  */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USARTx configured as follows:
	- BaudRate = 115200 baud  
	- Word Length = 8 Bits
	- One Stop Bit
	- No parity
	- Hardware flow control disabled (RTS and CTS signals)
	- Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* USART configuration */
	USART_Init(USART2, &USART_InitStructure);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 

	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
	{}
	
	/* NVIC configuration */
	/* Enable the USARRx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable USART */
	USART_Cmd(USART2, ENABLE);


	printf("\n\rUSART Printf Example: retarget the C library printf function to the USART\n\r");


}

void SysTickConfig(void)
{
	RCC_ClocksTypeDef RCC_Clocks;
	/* This function fills the RCC_ClockFreq structure with the current
	 frequencies of different on chip clocks (for debug purpose) */	
	RCC_GetClocksFreq(&RCC_Clocks);
	
	printf("===========================\r\n");
	printf("SYSCLK_Frequency = %d Hz\n\r",RCC_Clocks.SYSCLK_Frequency);	
	printf("AHB = %d Hz\n\r",RCC_Clocks.HCLK_Frequency);
	printf("APB1 = %d Hz\n\r",RCC_Clocks.PCLK1_Frequency);
	printf("APB2 = %d Hz\n\r",RCC_Clocks.PCLK2_Frequency);
	
	/* Setup SysTick Timer for 1ms interrupts  */
	if (SysTick_Config(SystemCoreClock / 1000))
	{
		/* Capture error */
		while (1);
	}
	
	/* Configure the SysTick handler priority */
	NVIC_SetPriority(SysTick_IRQn, 0x01);
	
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t uTime)
{ 
	uwTimingDelay = uTime;
	while(uwTimingDelay != 0);
}


/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (uwTimingDelay != 0x00)
  { 
    uwTimingDelay--;
  }
}

//currently not use
/*

void SystemClkDelay(void)
{
	uint32_t i;
	i = 0xffff;
	while(i--);
}

void wtPutChar(uint8_t ccc)
{
	UART1_SendData8(ccc);
	// Loop until the end of transmission 
	while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET);	
}

u16 GetAbsTime(u16 a,u16 b)
{
	u16 c;
	if(a>=b) c=(a-b);
	else c=65535-(b-a);	
	
	return c;
}
*/
uint8_t UART_GetByte(void)
{
	while ( USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
	{
	}
	return (uint8_t)USART_ReceiveData(USART2);
}

void UART_SendByte(uint8_t Data)
{
	USART_SendData(USART2 , (unsigned char)Data);
	while (USART_GetFlagStatus(USART2 , USART_FLAG_TC)==RESET);
	{
	}
}

void UART_SendString(uint8_t* Data,uint16_t len)
{
	#if 1
	uint16_t i=0;
	for(i=0;i<len;i++ )
	{
		UART_SendByte(Data[i]);
	}
	#else	//ignore len
    while(*Data)  
    {  
        USART_SendData(USART2, (unsigned char) *Data++);  
        /* Loop until the end of transmission */  
        while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);  //USART_FLAG_TXE
    } 
	#endif
}

void SystemClkDelay(uint32_t u32Delay)
{
	//uint32_t i;
	//i = 0xffff;
	while(u32Delay--);
}


