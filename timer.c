
#include "timer.h"
//#include "stm32f4xx_rcc.h"
//#include "stm32f4xx_tim.h"
//#include "misc.h"
#include "main.h"


void TIM2_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitTypeDef base_timer;
	TIM_TimeBaseStructInit(&base_timer);

	/* Делитель учитывается как TIM_Prescaler + 1, поэтому отнимаем 1 */
	base_timer.TIM_Prescaler = 10 - 1; 	// делитель 24
	//base_timer.TIM_Period = 72;
	//base_timer.TIM_Period = 524;
	base_timer.TIM_Period = 1048;
	TIM_TimeBaseInit(TIM2, &base_timer);

  /* Разрешаем прерывание по обновлению (в данном случае -
   * по переполнению) счётчика таймера TIM2. */
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	/* Включаем таймер */
	TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		IRQ_timer();
	};
}

void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM2_Start(void)
{
   TIM2_Config();
   NVIC_Config();
}

void TIM2_Stop(void)
{

}
