
#include "codec.h"
#include "speex.h"


short in_short[FRAME_SIZE];
signed short out_short[FRAME_SIZE];
BYTE in_byte[SPEEX_SIZE];

void *pDec = 0;
SpeexBits inbits;
SpeexBits obits;

void *pEnc = 0;

void createEncoder(void)
{
	signed int tmp = 0;
	speex_bits_init(&inbits);
	pEnc = speex_encoder_init(&speex_nb_mode);
	//tmp=0;
	//speex_encoder_ctl(pEnc, SPEEX_SET_VBR, &tmp);
	tmp=4;
	speex_encoder_ctl(pEnc, SPEEX_SET_QUALITY, &tmp);
	tmp=2;
	speex_encoder_ctl(pEnc, SPEEX_SET_COMPLEXITY, &tmp);
}

BYTE* encodeSPEEX(signed short *data)
{
	char *pCh = (char*)(&in_byte[0]);
	speex_bits_reset(&inbits);
	speex_encode_int(pEnc, data, &inbits);
	speex_bits_write(&inbits, pCh, SPEEX_SIZE);
	return (BYTE*)pCh;
}

void createDecoder(void)
{
	pDec = speex_decoder_init(&speex_nb_mode);
	speex_bits_init(&obits);
}

signed short* decodeSPEEX(BYTE *data)
{
	speex_bits_read_from(&obits, (char*)data, SPEEX_SIZE);
	speex_decode_int(pDec, &obits, out_short);
	speex_bits_reset(&obits);
	return (signed short*)(&out_short[0]);
}

void deleteEncoder(void)
{

}

void deleteDecoder(void)
{

}


