/*
**
**                           main.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 00 $
   Last changed by:    $Author: $
   Last changed date:  $Date:  $
   ID:                 $Id:  $

**********************************************************************/


//#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"
//#include "speechOpus.h"
#include "voice.h"

BYTE spx_data[SPXSZ];
signed short pcm_dataA[FRAME_SIZE];
signed short pcm_dataB[FRAME_SIZE];
volatile char keyAB = 0;

char recFlg = 0;
volatile char sendFlg = 0;
volatile static int fileCntr = 0;
volatile BYTE bFlg = 0;
volatile BYTE wrFlg = 1;


void initWord(void)
{
	int i;
	pcmbff.counter = 0;
	pcmbff.wxcnt = 0;
	pcmbff.rxcnt = 0;
	pcmbff.overfl = 0;
	for(i=0; i<PCM_BFF_SZ; i++)
	{
		pcmbff.buff[i] = 0;
	};
}

void putWord(signed short b)
{
	if(++pcmbff.counter <= PCM_BFF_SZ)
	{
		pcmbff.buff[pcmbff.wxcnt] = b;
		if(++pcmbff.wxcnt >= PCM_BFF_SZ) pcmbff.wxcnt = 0;
	}
	else
	{
		pcmbff.counter = PCM_BFF_SZ;
		pcmbff.overfl = 1;
	};
}

signed short getWord(void)
{
	signed short res = 0;
	if(pcmbff.counter > 0)
	{
		res = pcmbff.buff[pcmbff.rxcnt];
		if(++pcmbff.rxcnt >= PCM_BFF_SZ) pcmbff.rxcnt = 0;
		--pcmbff.counter;
	};
	return res;
}

WORD checkWord(void)
{
	return pcmbff.counter;
}


/******************* OPUS STREAMING ********************/
void initByte(void)
{
	int i;
	inSpxBff.counter = 0;
	inSpxBff.wxcnt = 0;
	inSpxBff.rxcnt = 0;
	inSpxBff.overfl = 0;
	for(i=0; i<SPEEX_BFF_SZ; i++)
	{
		inSpxBff.buff[i] = 0;
	};
}

void putByte(BYTE b)
{
    if(++inSpxBff.counter <= SPEEX_BFF_SZ)
    {
   	 inSpxBff.buff[inSpxBff.wxcnt] = b;
       if(++inSpxBff.wxcnt >= SPEEX_BFF_SZ) inSpxBff.wxcnt = 0;
    }
    else
    {
   	 inSpxBff.counter = SPEEX_BFF_SZ;
   	 inSpxBff.overfl = 1;
    };
}

BYTE getByte(void)
{
    BYTE res = 0;
    if(inSpxBff.counter > 0)
    {
        res = inSpxBff.buff[inSpxBff.rxcnt];
        if(++inSpxBff.rxcnt >= SPEEX_BFF_SZ) inSpxBff.rxcnt = 0;
        --inSpxBff.counter;
    };
    return res;
}

WORD checkByte(void)
{
    return inSpxBff.counter;
}

/****************** !OPUS STREAMING! *******************/

void IRQ_timer(void) // Ft = 8kHz
{
	//static int divider = 0;
	//static int inCntr = 0;

	//if(recFlg == 1)
	//{
		//if(++divider >= 160) // Ft = 50Hz
		//{
			//putByte((BYTE)(speech[inCntr]));
			//if(++inCntr >= speechLen) inCntr = 0;

			//getFromFileSpx((BYTE *)(&spx_data[0]));
			//bFlg = 1;

			//divider = 0;
			//debugBlinkOrangeLed(50);
		//};
	//};
}

signed short sin_m[16] =
{
	0, 383, 707, 924, 1000, 933, 707, 383,
	0, -383, -707, -924, -1000, -933, -707, -383
};

void IRQ_mic(WORD *bff, WORD sz) // Ft = 500Hz
{
	WORD i=0;
	signed short tmp;
	if(recFlg == 1)
	{
		for(i=0; i<sz; i++)
		{
			if(bff[i] < 0x7FFF)
			{
				tmp = (signed short)0x7FFF - (signed short)bff[i];
			}
			else if (bff[i] == 0x7FFF)
			{
				tmp = 0;
			}
			else
			{
				tmp = (signed short)bff[i] - (signed short)0x7FFF;
			};
			//tmp /= 3;
			putWord(tmp);
		};
		debugBlinkRedLed(500);
	};
}

void IRQ_play(void)
{
	static short tmpd = 0;
   static char oddFlgG = 0;
   static WORD iPCM = 0;

   if(oddFlgG == 0)
   {
      oddFlgG = 1;
      if(keyAB == 0) tmpd = pcm_dataB[iPCM++];
      else tmpd = pcm_dataA[iPCM++];
      if(iPCM >= FRAME_SIZE)
      {
      	iPCM = 0;
      	wrFlg = 1;
      	if(keyAB == 0) keyAB = 1;
      	else keyAB = 0;
      };
		debugBlinkBlueLed(8000);
   }
   else oddFlgG = 0;

   //debugBlinkBlueLed(16000);

   sendSample(tmpd/3);
   //sendSample(0);
}

void getFromFileSpx(BYTE *pack)
{
	BYTE i;
	int cnt = fileCntr*SPXSZ;
	for(i=0; i<SPXSZ; i++)
	{
		if(cnt < male_voice_len) pack[i] = male_voice[cnt++];
	};
	if(++fileCntr >= male_voice_frames) fileCntr = 0;
}

int main(void)
{
	//char codecIsOn = 0;
	char codecIsOn = 1;
	signed short *pOP = 0;
	//signed short pTmp[200];
	signed short *pSPX = 0;
	BYTE encBff[100];
	//BYTE *pOpEnc = (BYTE *)(&encBff[0]);
	WORD n;
	signed short pcmBff[200];
	signed short *pPcm = (signed short *)(&pcmBff[0]);
	BYTE *pOut = (BYTE *)(&encBff[0]);

	SystemInit();
	initPlayer(80, 8000);
	TIM2_Start();
	initGPIO_LED();
	initMic();
	//initByte();

	createDecoder();
	createEncoder();


	SystemCoreClockUpdate();

	sendSample(0);

	recFlg = 1;
	while(1)
	{
		if(wrFlg == 1) // Если буфер воспроизведения пуст Ft = 500Hz (160smpl =>)
		{
			if(checkWord() >= FRAME_SIZE)
			{
				wrFlg = 0;

				//pOP = pTmp;
				//pOut = pTmp;
				for(n=0; n<FRAME_SIZE; n++)
				{
					pPcm[n] = getWord();
				};

				if(codecIsOn == 1) pOut = encodeSPEEX(pPcm);

				//getFromFileSpx(pOut);
				if(codecIsOn == 1) pOP = decodeSPEEX(pOut);
				else pOP = pPcm;

				if(keyAB == 0) pSPX = (signed short*)(&pcm_dataA[0]);
				else pSPX = (signed short*)(&pcm_dataB[0]);

				for(n=0; n<FRAME_SIZE; n++)
				{
					pSPX[n] = pOP[n];
				};
				wrFlg = 0;
				debugBlinkGreenLed(50);
			};
			//debugBlinkOrangeLed(500);
		};
	};
}



//if(bFlg == 1)
//{
	//for(n=0; n<SPXSZ; n++)
	//{
		//putByte(spx_data[n]);
		//pOpEnc[n] = spx_data[n];
	//};
	//bFlg = 0;
	//if(wrFlg == 1)
	//{
		//pOP = decodeSPEEX(pOpEnc);
		//for(n=0; n<FRAME_SIZE; n++)
		//{
			//pcm_data[n] = pOP[n];
		//};
		//wrFlg = 0;
		//debugBlinkGreenLed(100);
	//};
//};
//debugBlinkGreenLed(500000);
