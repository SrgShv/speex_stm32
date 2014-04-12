/*
**
**                           SndPlay.h
**
**
**********************************************************************/

#ifndef __SNDPLAY_H
#define __SNDPLAY_H

typedef unsigned int    DWORD;
typedef unsigned short  WORD;
typedef unsigned char   BYTE;

#define CODEC_FLAG_TIMEOUT             ((uint32_t)0x1000)
#define CODEC_LONG_TIMEOUT             ((uint32_t)(300 * CODEC_FLAG_TIMEOUT))
#define CODEC_ADDRESS                   0x94  /* b00100111 */
/* Mask for the bit EN of the I2S CFGR register */
#define I2S_ENABLE_MASK                 0x0400

void initPlayer(BYTE volume, WORD Fs);
DWORD Codec_WriteRegister(BYTE RegisterAddr, BYTE RegisterValue);
DWORD Codec_TIMEOUT_UserCallback(void);
void initCS43L22(BYTE volume);
DWORD Codec_ReadRegister(BYTE RegisterAddr);
void sendSample(signed short smpl);
char checkSPITXE(void);

extern void IRQ_play(void);

DWORD dbg_check1(void);
DWORD dbg_check2(void);


#endif /* __SNDPLAY_H */
