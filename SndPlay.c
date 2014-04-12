/*
**
**                           SndPlay.c
**
**
**********************************************************************/

#include "SndPlay.h"
//#include "stm32f4xx_rcc.h"
//#include "stm32f4xx_gpio.h"
//#include "stm32f4xx_i2c.h"
//#include "stm32f4xx_spi.h"
//#include "misc.h"
#include "main.h"

volatile DWORD CODECTimeout = 1228800;


void initPlayer(BYTE volume, WORD Fs)
{
   DWORD iCntr = 0;
   /* configures I2S PLL module */
   RCC_PLLI2SCmd(DISABLE);
	switch(Fs)
	{
		case 8000:
		   RCC_PLLI2SConfig(256,5);
			//RCC_PLLI2SConfig(180,2);
		   break;
		case 16000:
		   RCC_PLLI2SConfig(213,4);
			//RCC_PLLI2SConfig(180,2);
		   break;
		case 11025:
		   RCC_PLLI2SConfig(429,4);
		   break;
		case 32000:
		   RCC_PLLI2SConfig(426,4);
		   break;
		case 48000:
		   RCC_PLLI2SConfig(258,3);
		   break;
		case 22050:
		   RCC_PLLI2SConfig(429,4);
		   break;
		case 44100:
		   RCC_PLLI2SConfig(271,6);
		   break;
	}
	RCC_PLLI2SCmd(ENABLE);

	/**====================================================================*/

	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable Reset GPIO Clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/* Audio reset pin configuration -------------------------------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Enable I2S and I2C GPIO clocks */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOA, ENABLE);

	/** I2C control: CS43L22 - I2C address 0x94 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* Connect pins to I2C peripheral */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);

	/** DATA SOUND I2S - CS43L22 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Connect pins to I2S peripheral  */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);

	/* CODEC_I2S pins configuration: MCK pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* Connect pins to I2S peripheral  */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_SPI3);

	/**========================================================================*/
	/*-------------------------- CODEC RESET ----------------------------------*/
	/* Power Down the codec */
	GPIO_WriteBit(GPIOD, GPIO_Pin_4, Bit_RESET);

	/* wait for a delay to insure registers erasing */
	for(iCntr = 0; iCntr < 100000; iCntr++)
   {
      asm("nop");
   };

	/* Power on the codec */
	GPIO_WriteBit(GPIOD, GPIO_Pin_4, Bit_SET);

	/**========================================================================*/
	/*-------------------------- SET CS43L22 CONTROL I2C ----------------------*/
	I2C_InitTypeDef I2C_InitStructure;
	/* Enable the CODEC_I2C peripheral clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	/* CODEC_I2C peripheral configuration */
	I2C_DeInit(I2C1);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x33;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 100000;
	/* Enable the I2C peripheral */
	I2C_Cmd(I2C1, ENABLE);
	I2C_Init(I2C1, &I2C_InitStructure);

	/**========================================================================*/
	/*-------------------------- CS43L22 INIT ---------------------------------*/

	initCS43L22(volume);

	/**========================================================================*/
	/*------------------ Configure the I2S peripheral -------------------------*/

	I2S_InitTypeDef I2S_InitStructure;

   /* Enable the CODEC_I2S peripheral clock */
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);

   /* CODEC_I2S peripheral configuration */
   SPI_I2S_DeInit(SPI3);
   //I2S_InitStructure.I2S_AudioFreq = Fs;
	I2S_InitStructure.I2S_AudioFreq = 8000;
   //I2S_InitStructure.I2S_Standard = I2S_Standard_PCMLong;
   //I2S_InitStructure.I2S_Standard = I2S_Standard_LSB;
   //I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;
   I2S_InitStructure.I2S_Standard = I2S_Standard_MSB;
   I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
   I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
   I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;
   I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable;

   /* Initialize the I2S peripheral with the structure above */
   I2S_Init(SPI3, &I2S_InitStructure);
   
   /** --------------------------------------------------------------------- */
   NVIC_InitTypeDef NVIC_InitStructure;

   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3); 
   /* Configure the SPI interrupt priority */
   NVIC_InitStructure.NVIC_IRQChannel = SPI3_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
   
   SPI_I2S_ITConfig(SPI3, SPI_I2S_IT_TXE, ENABLE);
   

   /* If the I2S peripheral is still not enabled, enable it */
   if ((SPI3->I2SCFGR & I2S_ENABLE_MASK) == 0)
   {
      I2S_Cmd(SPI3, ENABLE);
   };
}

void SPI3_IRQHandler(void)
{
   IRQ_play();
}

DWORD dbg_check1(void)
{
	 return SPI3->I2SPR;
}

DWORD dbg_check2(void)
{
	 return SPI3->I2SCFGR;
}

void sendSample(signed short smpl)
{
	 //if(SPI3->SR & SPI_I2S_FLAG_TXE) SPI3->DR = smpl;
	 SPI3->DR = smpl;
}

char checkSPITXE(void)
{
	char res = 0;
	if(SPI3->SR & SPI_I2S_FLAG_TXE) res = 1;
	return res;
}

DWORD Codec_WriteRegister(BYTE RegisterAddr, BYTE RegisterValue)
{
   DWORD result = 0;

   /*!< While the bus is busy */
   CODECTimeout = CODEC_LONG_TIMEOUT;
   while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
   {
      if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
   };

   /* Start the config sequence */
   I2C_GenerateSTART(I2C1, ENABLE);

   /* Test on EV5 and clear it */
   CODECTimeout = CODEC_FLAG_TIMEOUT;
   while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
   {
      if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
   };

   /* Transmit the slave address and enable writing operation */
   I2C_Send7bitAddress(I2C1, CODEC_ADDRESS, I2C_Direction_Transmitter);

   /* Test on EV6 and clear it */
   CODECTimeout = CODEC_FLAG_TIMEOUT;
   while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
   {
      if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
   };

   /* Transmit the first address for write operation */
   I2C_SendData(I2C1, RegisterAddr);

   /* Test on EV8 and clear it */
   CODECTimeout = CODEC_FLAG_TIMEOUT;
   while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
   {
      if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
   };

   /* Prepare the register value to be sent */
   I2C_SendData(I2C1, RegisterValue);

   /*!< Wait till all data have been physically transferred on the bus */
   CODECTimeout = CODEC_LONG_TIMEOUT;
   while(!I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF))
   {
      if((CODECTimeout--) == 0) Codec_TIMEOUT_UserCallback();
   };

   /* End the configuration sequence */
   I2C_GenerateSTOP(I2C1, ENABLE);

   /* Verify that the data has been correctly written */
   result = (Codec_ReadRegister(RegisterAddr) == RegisterValue)? 0:1;

   /* Return the verifying value: 0 (Passed) or 1 (Failed) */
   return result;
}

DWORD Codec_TIMEOUT_UserCallback(void)
{
  /* Block communication and all processes */
  while (1)
  {
  };
}

void initCS43L22(BYTE volume)
{
   DWORD i;
   for(i = 0; i < 1000000; i++)
   {
      asm("nop");
      asm("nop");
      asm("nop");
      asm("nop");
      asm("nop");
   };
   /* Keep Codec powered OFF */
	Codec_WriteRegister(0x02, 0x01);

   /* SPK always OFF & HP always ON */
	Codec_WriteRegister(0x04, 0xAF);

	/* Clock configuration: Auto detection */
	Codec_WriteRegister(0x05, 0x81);

	/* Set the Slave Mode and the audio Standard */
	Codec_WriteRegister(0x06, 0x03);

	/* Set the Master volume */
   Codec_WriteRegister(0x20, 0x00);
   Codec_WriteRegister(0x21, 0x00);

	/* Power on the Codec */
	Codec_WriteRegister(0x02, 0x9E);

	/* Additional configuration for the CODEC. These configurations are done to reduce
	  the time needed for the Codec to power off. If these configurations are removed,
	  then a long delay should be added between powering off the Codec and switching
	  off the I2S peripheral MCLK clock (which is the operating clock for Codec).
	  If this delay is not inserted, then the codec will not shut down properly and
	  it results in high noise after shut down. */

	/* Disable the analog soft ramp */
	Codec_WriteRegister(0x0A, 0x00);

   /* Disable the digital soft ramp */
   Codec_WriteRegister(0x0E, 0x04);

	/* Disable the limiter attack level */
	Codec_WriteRegister(0x27, 0x00);
	/* Adjust Bass and Treble levels */
	Codec_WriteRegister(0x1F, 0x0F);
	/* Adjust PCM volume level */
	Codec_WriteRegister(0x1A, 0x0A);
	Codec_WriteRegister(0x1B, 0x0A);
}

DWORD Codec_ReadRegister(BYTE RegisterAddr)
{
   DWORD result = 0;

   /*!< While the bus is busy */
   CODECTimeout = CODEC_LONG_TIMEOUT;
   while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
   {
      if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
   }

   /* Start the config sequence */
   I2C_GenerateSTART(I2C1, ENABLE);

   /* Test on EV5 and clear it */
   CODECTimeout = CODEC_FLAG_TIMEOUT;
   while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
   {
      if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
   }

   /* Transmit the slave address and enable writing operation */
   I2C_Send7bitAddress(I2C1, CODEC_ADDRESS, I2C_Direction_Transmitter);

   /* Test on EV6 and clear it */
   CODECTimeout = CODEC_FLAG_TIMEOUT;
   while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
   {
      if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
   }

   /* Transmit the register address to be read */
   I2C_SendData(I2C1, RegisterAddr);

   /* Test on EV8 and clear it */
   CODECTimeout = CODEC_FLAG_TIMEOUT;
   while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF) == RESET)
   {
      if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
   }

   /*!< Send START condition a second time */
   I2C_GenerateSTART(I2C1, ENABLE);

   /*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
   CODECTimeout = CODEC_FLAG_TIMEOUT;
   while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
   {
      if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
   }

   /*!< Send Codec address for read */
   I2C_Send7bitAddress(I2C1, CODEC_ADDRESS, I2C_Direction_Receiver);

   /* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
   CODECTimeout = CODEC_FLAG_TIMEOUT;
   while(I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR) == RESET)
   {
      if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
   }

   /*!< Disable Acknowledgment */
   I2C_AcknowledgeConfig(I2C1, DISABLE);

   /* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
   (void)I2C1->SR2;

   /*!< Send STOP Condition */
   I2C_GenerateSTOP(I2C1, ENABLE);

   /* Wait for the byte to be received */
   CODECTimeout = CODEC_FLAG_TIMEOUT;
   while(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET)
   {
      if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
   }

   /*!< Read the byte received from the Codec */
   result = I2C_ReceiveData(I2C1);

   /* Wait to make sure that STOP flag has been cleared */
   CODECTimeout = CODEC_FLAG_TIMEOUT;
   while(I2C1->CR1 & I2C_CR1_STOP)
   {
      if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
   }

   /*!< Re-Enable Acknowledgment to be ready for another reception */
   I2C_AcknowledgeConfig(I2C1, ENABLE);

   /* Clear AF flag for next communication */
   I2C_ClearFlag(I2C1, I2C_FLAG_AF);

   /* Return the byte read from Codec */
   return result;
}


