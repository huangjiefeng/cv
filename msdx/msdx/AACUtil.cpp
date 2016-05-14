#include "StdAfx.h"
#include "AACUtil.h"

#include "3rd\faac\aac_types.h"
#include "3rd\faac\faac.h"


#define BIT_RATE 96000

AACUtil::AACUtil(void)
{
}

AACUtil::~AACUtil(void)
{
}

// 注意以下两个函数返回配置的一致性

AACConfig AACUtil::GetAACEncConfig()
{
	AACConfig cfg = {0};
	cfg.version = AAC_VERSION_MPEG4;
	cfg.object_type = AAC_OBJECT_LOW;
	cfg.output_type = AAC_OUTPUT_RAW;
	cfg.bitrate = BIT_RATE;

	return cfg;
}

void AACUtil::GetDecoderSpecificInfo( unsigned char **ppBuffer, unsigned long &nSize, int nSampleRate, int nChannels, int nBitsPerSample )
{
	*ppBuffer = NULL;
	nSize = 0;

	AACInfo info = {0};
	info.samplerate = nSampleRate;
	info.channels = nChannels;

	unsigned long	framesize;
	unsigned long	ob;

	faacEncHandle encoder = faacEncOpen(info.samplerate, info.channels, &framesize, &ob);
	if (!encoder) return;

	info.frame_size = framesize;

	faacEncConfigurationPtr	conf;
	conf = faacEncGetCurrentConfiguration(encoder);
	if (!conf) {
		faacEncClose(encoder);
		encoder = NULL;
		return;
	}

	conf->mpegVersion	= 4; // MPEG_4
	conf->aacObjectType	= 2; // LC_AAC
	conf->allowMidside	= 0;
	conf->bandWidth		= 0;
	conf->bitRate		= BIT_RATE / info.channels;
	conf->inputFormat	= FAAC_INPUT_16BIT;
	conf->useLfe		= 0;
	conf->useTns		= 0;
	conf->shortctl		= SHORTCTL_NORMAL;
	conf->outputFormat	= 0; // RAW
	faacEncSetConfiguration(encoder, conf);

	BYTE *ex_ptr = NULL;
	unsigned long ex_size;
	faacEncGetDecoderSpecificInfo(encoder, &ex_ptr, &ex_size);
	if (ex_ptr && ex_size > 0) {
		nSize = ex_size;
		*ppBuffer = (BYTE*)malloc(ex_size);
		memcpy(*ppBuffer, ex_ptr, ex_size);
	}
	faacEncClose(encoder);
}

void AACUtil::FreeDecoderSpecificBuffer( unsigned char *buf )
{
    //因为上面用的malloc，这里必须使用free来释放。外部应调用此方法释放
    free(buf);
}
