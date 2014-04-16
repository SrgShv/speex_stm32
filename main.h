/*
**
**                           main.h
**
**
**********************************************************************/

#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx_conf.h"
//#include "structs.h"
//#include "tables.h"
//#include "control.h"

typedef unsigned int    DWORD;
typedef unsigned short  WORD;
typedef unsigned char   BYTE;

#define PCMSZ           480
#define WPCMSZ          (PCMSZ*2)
#define SPBSZ           200
#define SINSZ            32
#define BFFSZ           1000
#define SPXSZ            20
#define SPEEX_BFF_SZ 	200
#define FRAME_SIZE 		160
#define PCM_BFF_SZ      480

struct CRKLPCM
{
   signed short buff[PCM_BFF_SZ];
   BYTE overfl;
   WORD counter;
   WORD rxcnt;
   WORD wxcnt;
} pcmbff;

struct STREAMBUFF
{
   BYTE buff[SPEEX_BFF_SZ];
   BYTE overfl;
   WORD counter;
   WORD rxcnt;
   WORD wxcnt;
} inSpxBff;

#define OPUS_TEST 		480
short buffOUTPCM[OPUS_TEST*2+1];

/** start initialize PCM, SPX structures */
void IRQ_timer(void);
void IRQ_mic(WORD *bff, WORD sz);
void IRQ_play(void);

void writeToBff(signed short data);
signed short readFromBff(void);
WORD checkBff(void);


extern void TIM2_Start(void);
extern void initGPIO_LED(void);
extern void debugBlinkBlueLed(const int divScaler);
extern void debugBlinkRedLed(const int divScaler);
extern void debugBlinkOrangeLed(const int divScaler);
extern void debugBlinkGreenLed(const int divScaler);
extern void initPlayer(BYTE volume, WORD Fs);
extern void sendSample(signed short smpl);
extern char checkSPITXE(void);
extern void initMic(void);

/************************* speex codec interface ******************************/
extern void createEncoder(void);
extern BYTE* encodeSPEEX(signed short *data);
extern void createDecoder(void);
extern void deleteEncoder(void);
extern signed short* decodeSPEEX(BYTE *data);

void initByte(void);
void putByte(BYTE b);
BYTE getByte(void);
WORD checkByte(void);
BYTE parseOpusInStream(BYTE b);
void initWord(void);
void putWord(signed short b);
signed short getWord(void);
WORD checkWord(void);

void getFromFileSpx(BYTE *pack);

#define LIM_N       5
#define MULT_L0     10000
#define MULT_L1     17783

struct Limiter
{
    int Ulim;
    int Utrs;
    int Ui[LIM_N];
    int Uo[LIM_N];
    int K[LIM_N];
} slim;

void initLimiter(void);
signed short limit(signed short dIn);

#endif /* __MAIN_H */
