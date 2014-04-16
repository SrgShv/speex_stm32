/*
**
**                           SndMic.c
**
**
**********************************************************************/

#include "SndMic.h"
#include "pdm_filter.h"
//#include "stm32f4xx_rcc.h"
//#include "stm32f4xx_gpio.h"
//#include "stm32f4xx_spi.h"
//#include "misc.h"
#include "main.h"

PDMFilter_InitStruct Filter;
WORD InternalBuffer[INTERNAL_BUFF_SIZE];
DWORD InternalBufferSize = 0;
WORD RecBuf[PCM_OUT_SIZE];
WORD RecBuf1[PCM_OUT_SIZE];

void initMic(void)
{
   /* Enable CRC module */
   RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;

   /* Filter LP & HP Init */
   //Filter.LP_HZ = 6000;
   //Filter.HP_HZ = 100;
   Filter.LP_HZ = 0;
   Filter.HP_HZ = 0;

   Filter.Fs = 16000;
   Filter.Out_MicChannels = 1;
   Filter.In_MicChannels = 1;

   PDM_Filter_Init((PDMFilter_InitStruct *)&Filter);

   /** --------------------------------------------------------------------- */
   GPIO_InitTypeDef GPIO_InitStructure;

   /* Enable GPIO clocks */
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);

   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

   /* SPI SCK pin configuration */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
   GPIO_Init(GPIOB, &GPIO_InitStructure);

   /* Connect SPI pins to AF5 */  
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_SPI2);

   /* SPI MOSI pin configuration */
   GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;
   GPIO_Init(GPIOC, &GPIO_InitStructure);
   GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_SPI2);
   
   /** --------------------------------------------------------------------- */
   NVIC_InitTypeDef NVIC_InitStructure;

   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3); 
   /* Configure the SPI interrupt priority */
   NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
   
   /** --------------------------------------------------------------------- */
   I2S_InitTypeDef I2S_InitStructure;

   /* Enable the SPI clock */
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
   /* SPI configuration */
   SPI_I2S_DeInit(SPI2);
   I2S_InitStructure.I2S_AudioFreq = 16000;
   I2S_InitStructure.I2S_Standard = I2S_Standard_MSB;
   I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
   I2S_InitStructure.I2S_CPOL = I2S_CPOL_High;
   I2S_InitStructure.I2S_Mode = I2S_Mode_MasterRx;
   I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
   /* Initialize the I2S peripheral with the structure above */
   I2S_Init(SPI2, &I2S_InitStructure);
   /* Enable the Rx buffer not empty interrupt */
   SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);
   /** --------------------------------------------------------------------- */
   /* Enable the SPI peripheral */
   I2S_Cmd(SPI2, ENABLE); 
}

/** Getting data from microphone and filterring PDM to PCM */
void SPI2_IRQHandler(void) 
{  
	WORD volume;//, i;
	WORD app;
	//signed short tmp;
	
	/* Check if data are available in SPI Data register */
	if (SPI_GetITStatus(SPI2, SPI_I2S_IT_RXNE) != RESET)
	{
		app = SPI_I2S_ReceiveData(SPI2);
		InternalBuffer[InternalBufferSize++] = ((((WORD)(app) & 0xFF00) >> 8) | (((WORD)(app) & 0x00FF) << 8));
		/* Check to prevent overflow condition */
		if (InternalBufferSize >= INTERNAL_BUFF_SIZE) // 64
		{
			InternalBufferSize = 0;
			volume = 100;
			PDM_Filter_64_LSB((BYTE *)InternalBuffer, (WORD *)RecBuf, volume , (PDMFilter_InitStruct *)&Filter);

			//for(i=0; i<PCM_OUT_SIZE; i++)
			//{
				//tmp = (signed short)RecBuf[i];
				//RecBuf[i] = (WORD)(tmp>>2);
			//};

         IRQ_mic((WORD *)RecBuf, PCM_OUT_SIZE);
		};
	};
}




