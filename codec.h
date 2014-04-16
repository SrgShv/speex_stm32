/*
**
**                           codec.h
**
**
**********************************************************************/

#ifndef __CODEC_H
#define __CODEC_H

typedef unsigned int    DWORD;
typedef unsigned short  WORD;
typedef unsigned char   BYTE;

#define FRAME_SIZE 160
#define SPEEX_SIZE 20

//void initDecoder(void);

void createEncoder(void);
BYTE* encodeSPEEX(signed short *data);
void createDecoder(void);
void deleteEncoder(void);
signed short* decodeSPEEX(BYTE *data);
void deleteDecoder(void);

#endif /* __CODEC_H */
