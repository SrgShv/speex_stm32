
//#include "main.h"
#include "codec.h"
#include "speex.h"
#include "speex_callbacks.h"

short in_short[FRAME_SIZE];
signed short out_short[FRAME_SIZE];
BYTE in_byte[SPEEX_SIZE];
//char arrStdErr[100];
//char *stderr = (char*)(&arrStdErr[0]);
void *pDec = 0;
//SpeexCallback callback;
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
	tmp=1;
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
	signed int tmp = 0;
	//pDec = speex_decoder_init(speex_lib_get_mode(SPEEX_MODEID_NB));
	pDec = speex_decoder_init(&speex_nb_mode);
	//speex_decoder_ctl(pDec, SPEEX_SET_ENH, &tmp);
	speex_bits_init(&obits);
}

signed short* decodeSPEEX(BYTE *data)
{
	speex_bits_read_from(&obits, (char*)data, SPEEX_SIZE);
	speex_decode_int(pDec, &obits, out_short);
	speex_bits_reset(&obits);
	return (signed short*)(&out_short[0]);
}

//speex_decoder_ctl(pDec, SPEEX_SET_HIGHPASS, &tmp);

/* Turn this off if you want to measure SNR (on by default) */
//tmp=1;
//speex_decoder_ctl(pDec, SPEEX_SET_HIGHPASS, &tmp);
//speex_decoder_ctl(pDec, SPEEX_GET_LOOKAHEAD, &tmp);

/* BEGIN: You probably don't need the following in a real application */
//callback.callback_id = SPEEX_INBAND_CHAR;
//callback.func = speex_std_char_handler;
//callback.data = stderr;
//speex_decoder_ctl(pDec, SPEEX_SET_HANDLER, &callback);

//callback.callback_id = SPEEX_INBAND_MODE_REQUEST;
//callback.func = speex_std_mode_request_handler;
//callback.data = st;
//speex_decoder_ctl(pDec, SPEEX_SET_HANDLER, &callback);
/* END of unnecessary stuff */


