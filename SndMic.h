/*
**
**                           SndMic.h
**
**
**********************************************************************/

#ifndef __SNDMIC_H
#define __SNDMIC_H

typedef unsigned int    DWORD;
typedef unsigned short  WORD;
typedef unsigned char   BYTE;

/* PDM buffer input size */
#define INTERNAL_BUFF_SIZE      64

/* PCM buffer output size */
#define PCM_OUT_SIZE            16

void initMic(void);

extern void IRQ_mic(WORD *bff, WORD sz);

#endif /* __SNDMIC_H */